--- linux_old/drivers/net/ethernet/pilot_tulip/interrupt.c	2016-11-18 14:25:59.959970774 +0500
+++ linux/drivers/net/ethernet/pilot_tulip/interrupt.c	2016-11-18 14:24:41.501746841 +0500
@@ -0,0 +1,882 @@
+/*
+	drivers/net/tulip/interrupt.c
+
+	Copyright 2000,2001  The Linux Kernel Team
+	Written/copyright 1994-2001 by Donald Becker.
+
+	This software may be used and distributed according to the terms
+	of the GNU General Public License, incorporated herein by reference.
+
+	Please refer to Documentation/DocBook/tulip-user.{pdf,ps,html}
+	for more information on this driver.
+        Please submit bugs to http://bugzilla.kernel.org/ .
+
+*/
+
+#include <linux/pci.h>
+#include "tulip.h"
+#include <linux/etherdevice.h>
+
+int tulip_rx_copybreak;
+unsigned int tulip_max_interrupt_work;
+
+#define LC82C168 4 //Need to find the path and export accordingly
+
+#ifdef CONFIG_TULIP_NAPI_HW_MITIGATION
+#define MIT_SIZE 15
+#define MIT_TABLE 15 /* We use 0 or max */
+
+static unsigned int mit_table[MIT_SIZE+1] =
+{
+        /*  CRS11 21143 hardware Mitigation Control Interrupt
+            We use only RX mitigation we other techniques for
+            TX intr. mitigation.
+
+           31    Cycle Size (timer control)
+           30:27 TX timer in 16 * Cycle size
+           26:24 TX No pkts before Int.
+           23:20 RX timer in Cycle size
+           19:17 RX No pkts before Int.
+           16       Continues Mode (CM)
+        */
+
+        0x0,             /* IM disabled */
+        0x80150000,      /* RX time = 1, RX pkts = 2, CM = 1 */
+        0x80150000,
+        0x80270000,
+        0x80370000,
+        0x80490000,
+        0x80590000,
+        0x80690000,
+        0x807B0000,
+        0x808B0000,
+        0x809D0000,
+        0x80AD0000,
+        0x80BD0000,
+        0x80CF0000,
+        0x80DF0000,
+//       0x80FF0000      /* RX time = 16, RX pkts = 7, CM = 1 */
+        0x80F10000      /* RX time = 16, RX pkts = 0, CM = 1 */
+};
+#endif
+
+
+#ifdef  P3_A0
+#define SE_RD_DIV  16
+#else
+#define SE_RD_DIV  1
+#endif
+
+int tulip_refill_rx(struct net_device *dev)
+{
+	struct tulip_private *tp = netdev_priv(dev);
+	int entry;
+	int refilled = 0;
+
+	/* Refill the Rx ring buffers. */
+	for (; tp->cur_rx - tp->dirty_rx > 0; tp->dirty_rx++) {
+		entry = tp->dirty_rx % RX_RING_SIZE;
+		if (tp->rx_buffers[entry].skb == NULL) {
+			struct sk_buff *skb;
+			dma_addr_t mapping;
+
+			skb = tp->rx_buffers[entry].skb = dev_alloc_skb(PKT_BUF_SZ);
+			if (skb == NULL)
+				break;
+
+			mapping = pci_map_single(tp->pdev, skb->data, PKT_BUF_SZ,
+						 PCI_DMA_FROMDEVICE);
+			tp->rx_buffers[entry].mapping = mapping;
+
+			skb->dev = dev;			/* Mark as being used by this device. */
+			tp->rx_ring[entry].buffer1 = cpu_to_le32(mapping);
+			refilled++;
+		}
+		tp->rx_ring[entry].status = cpu_to_le32(DescOwned);
+	}
+#if 0
+	if(tp->chip_id == LC82C168) {
+		if(((ioread32(tp->base_addr + CSR5)>>17)&0x07) == 4) {
+			/* Rx stopped due to out of buffers,
+			 * restart it
+			 */
+			iowrite32(0x01, tp->base_addr + CSR2);
+		}
+	}
+#endif
+
+#ifdef P3_A0
+	iowrite32(0x01, tp->base_addr + CSR2);
+#endif
+	return refilled;
+}
+
+#ifdef CONFIG_TULIP_NAPI
+
+void oom_timer(unsigned long data)
+{
+	struct net_device *dev = (struct net_device *)data;
+	struct tulip_private *tp = netdev_priv(dev);
+	napi_schedule(&tp->napi);
+}
+
+int tulip_poll(struct napi_struct *napi, int budget)
+{
+	struct tulip_private *tp = container_of(napi, struct tulip_private, napi);
+	struct net_device *dev = tp->dev;
+	int entry = tp->cur_rx % RX_RING_SIZE;
+	int work_done = 0;
+#ifndef CONFIG_SOC_SE_PILOT4
+	int count = 0, entry_tmp = 0, i =0;
+#endif	
+#ifdef CONFIG_TULIP_NAPI_HW_MITIGATION
+	int received = 0;
+#endif
+
+#ifdef CONFIG_TULIP_NAPI_HW_MITIGATION
+
+/* that one buffer is needed for mit activation; or might be a
+   bug in the ring buffer code; check later -- JHS*/
+
+        if (budget >=RX_RING_SIZE) budget--;
+#endif
+
+	if (tulip_debug > 4)
+		printk(KERN_DEBUG " In tulip_rx(), entry %d %8.8x.\n", entry,
+			   tp->rx_ring[entry].status);
+
+       do {
+		if (ioread32(tp->base_addr + CSR5) == 0xffffffff) {
+			printk(KERN_DEBUG " In tulip_poll(), hardware disappeared.\n");
+			break;
+		}
+               /* Acknowledge current RX interrupt sources. */
+               iowrite32((RxIntr | RxNoBuf), tp->base_addr + CSR5);
+
+
+               /* If we own the next entry, it is a new packet. Send it up. */
+               while ( ! (tp->rx_ring[entry].status & cpu_to_le32(DescOwned))) {
+                       s32 status = le32_to_cpu(tp->rx_ring[entry].status);
+
+					   /* Pilot-II : Ignore Rx Collision Error */
+  						if ((status & 0xc8c2) == 0x8040)
+							status &= ~0x8040;
+
+                       if (tp->dirty_rx + RX_RING_SIZE == tp->cur_rx)
+                               break;
+
+                       if (tulip_debug > 5)
+                               printk(KERN_DEBUG "%s: In tulip_rx(), entry %d %8.8x.\n",
+                                      dev->name, entry, status);
+
+		       if (++work_done >= budget)
+                               goto not_done;
+
+                       if ((status & 0x38008300) != 0x0300) {
+                               if ((status & 0x38000300) != 0x0300) {
+                                /* Ingore earlier buffers. */
+                                       if ((status & 0xffff) != 0x7fff) {
+#ifndef CONFIG_SOC_SE_PILOT4									   
+		  // If it is a jumbo frame, get the total packet length and see how many descriptors it spans across.
+		  // Based on that clear the ownership bits and adjust the descriptor pointer to point to the correct
+		  // descriptor for the next frame
+                  #define LAST_DESC 0x100
+                  int k = 0;
+                  entry_tmp = entry;
+		  //count is valid only if Last desc is set,
+		  //ensure last desc is set and then proceed to drop the packet
+		  if(!(status & LAST_DESC)) {
+			for(k=0;k<100;k++) {
+				status = tp->rx_ring[entry_tmp].status;
+				if((status & LAST_DESC)) {
+					break;
+				}
+				udelay(1);
+			}
+		  }
+		  WARN(!(status & LAST_DESC), "NIC:LAST DESC Not set %x entry %d\n", status,entry);
+
+		  count = ((status >> 16) & 0x3fff) / PKT_BUF_SZ;
+		  count = (((status >> 16) & 0x3fff) % PKT_BUF_SZ) ? count+1 : count;
+		  for (i = 0; i < count; i++)
+		  {
+		     pci_unmap_single(tp->pdev, tp->rx_buffers[entry_tmp].mapping,PKT_BUF_SZ, PCI_DMA_FROMDEVICE);
+		     dev_kfree_skb_irq(tp->rx_buffers[entry_tmp].skb);
+		     tp->rx_buffers[entry_tmp].skb = NULL;
+		     tp->rx_buffers[entry_tmp].mapping = 0;
+
+		     // Clearing ownership bit
+		     tp->rx_ring[entry_tmp].status &= ~cpu_to_le32(DescOwned);
+		     entry_tmp = (entry_tmp+1) % RX_RING_SIZE;
+		  }
+		  // Increment the cur_rx count to (no. of desc -1) since it again gets incremented at the end of this functions.
+		  tp->cur_rx += --count;
+#endif
+                                               tp->stats.rx_length_errors++;
+                                       }
+                               } else if (status & RxDescFatalErr) {
+                                /* There was a fatal error. */
+                                       if (tulip_debug > 2)
+                                               printk(KERN_DEBUG "%s: Receive error, Rx status %8.8x.\n",
+                                                      dev->name, status);
+                                       tp->stats.rx_errors++; /* end of a packet.*/
+#if 0 
+                                       if (status & 0x0890) tp->stats.rx_length_errors++;
+                                       if (status & 0x0004) tp->stats.rx_frame_errors++;
+                                       if (status & 0x0002) tp->stats.rx_crc_errors++;
+                                       if (status & 0x0001) tp->stats.rx_fifo_errors++;									   
+#else
+                                       if (status & 0x0880) tp->stats.rx_length_errors++;
+									   if (status & 0x4000) tp->stats.rx_fifo_errors++;	
+                                       if (status & 0x0002) tp->stats.rx_crc_errors++;
+                                       if (status & 0x0001) tp->stats.rx_fifo_errors++;
+                                       if (status & 0x0040) tp->stats.collisions++;
+#endif									   
+                               }
+                       } else {
+                               /* Omit the four octet CRC from the length. */
+                               short pkt_len = ((status >> 16) & 0x7ff) - 4;
+                               struct sk_buff *skb;
+
+#ifndef final_version
+                               if (pkt_len > 1518) {
+                                       printk(KERN_WARNING "%s: Bogus packet size of %d (%#x).\n",
+                                              dev->name, pkt_len, pkt_len);
+                                       pkt_len = 1518;
+                                       tp->stats.rx_length_errors++;
+                               }
+#endif
+                               /* Check if the packet is long enough to accept without copying
+                                  to a minimally-sized skbuff. */
+                               if (pkt_len < tulip_rx_copybreak
+                                   && (skb = dev_alloc_skb(pkt_len + 2)) != NULL) {
+                                       skb_reserve(skb, 2);    /* 16 byte align the IP header */
+                                       pci_dma_sync_single_for_cpu(tp->pdev,
+								   tp->rx_buffers[entry].mapping,
+								   pkt_len, PCI_DMA_FROMDEVICE);
+#if ! defined(__alpha__)
+                                       skb_copy_to_linear_data(skb, tp->rx_buffers[entry].skb->data,
+                                                        pkt_len);
+                                       skb_put(skb, pkt_len);
+#else
+                                       memcpy(skb_put(skb, pkt_len),
+                                              tp->rx_buffers[entry].skb->data,
+                                              pkt_len);
+#endif
+                                       pci_dma_sync_single_for_device(tp->pdev,
+								      tp->rx_buffers[entry].mapping,
+								      pkt_len, PCI_DMA_FROMDEVICE);
+                               } else {        /* Pass up the skb already on the Rx ring. */
+                                       char *temp = skb_put(skb = tp->rx_buffers[entry].skb,
+                                                            pkt_len);
+
+#ifndef final_version
+                                       if (tp->rx_buffers[entry].mapping !=
+                                           le32_to_cpu(tp->rx_ring[entry].buffer1)) {
+                                               printk(KERN_ERR "%s: Internal fault: The skbuff addresses "
+                                                      "do not match in tulip_rx: %08x vs. %08llx %p / %p.\n",
+                                                      dev->name,
+                                                      le32_to_cpu(tp->rx_ring[entry].buffer1),
+                                                      (unsigned long long)tp->rx_buffers[entry].mapping,
+                                                      skb->head, temp);
+                                       }
+#endif
+
+                                       pci_unmap_single(tp->pdev, tp->rx_buffers[entry].mapping,
+                                                        PKT_BUF_SZ, PCI_DMA_FROMDEVICE);
+
+                                       tp->rx_buffers[entry].skb = NULL;
+                                       tp->rx_buffers[entry].mapping = 0;
+                               }
+                               skb->protocol = eth_type_trans(skb, dev);
+
+                               netif_receive_skb(skb);
+
+                               dev->last_rx = jiffies;
+                               tp->stats.rx_packets++;
+                               tp->stats.rx_bytes += pkt_len;
+                       }
+#ifdef CONFIG_TULIP_NAPI_HW_MITIGATION
+		       received++;
+#endif
+
+                       entry = (++tp->cur_rx) % RX_RING_SIZE;
+                       if (tp->cur_rx - tp->dirty_rx > RX_RING_SIZE/4)
+                               tulip_refill_rx(dev);
+
+                }
+
+               /* New ack strategy... irq does not ack Rx any longer
+                  hopefully this helps */
+
+               /* Really bad things can happen here... If new packet arrives
+                * and an irq arrives (tx or just due to occasionally unset
+                * mask), it will be acked by irq handler, but new thread
+                * is not scheduled. It is major hole in design.
+                * No idea how to fix this if "playing with fire" will fail
+                * tomorrow (night 011029). If it will not fail, we won
+                * finally: amount of IO did not increase at all. */
+       } while ((ioread32(tp->base_addr + CSR5) & RxIntr));
+
+ #ifdef CONFIG_TULIP_NAPI_HW_MITIGATION
+
+          /* We use this simplistic scheme for IM. It's proven by
+             real life installations. We can have IM enabled
+            continuesly but this would cause unnecessary latency.
+            Unfortunely we can't use all the NET_RX_* feedback here.
+            This would turn on IM for devices that is not contributing
+            to backlog congestion with unnecessary latency.
+
+             We monitor the device RX-ring and have:
+
+             HW Interrupt Mitigation either ON or OFF.
+
+            ON:  More then 1 pkt received (per intr.) OR we are dropping
+             OFF: Only 1 pkt received
+
+             Note. We only use min and max (0, 15) settings from mit_table */
+
+
+          if( tp->flags &  HAS_INTR_MITIGATION) {
+                 if( received > 1 ) {
+                         if( ! tp->mit_on ) {
+                                 tp->mit_on = 1;
+                                 iowrite32(mit_table[MIT_TABLE], tp->base_addr + CSR11);
+                         }
+                  }
+                 else {
+                         if( tp->mit_on ) {
+                                 tp->mit_on = 0;
+                                 iowrite32(0, tp->base_addr + CSR11);
+                         }
+                  }
+          }
+
+#endif /* CONFIG_TULIP_NAPI_HW_MITIGATION */
+
+         tulip_refill_rx(dev);
+
+         /* If RX ring is not full we are out of memory. */
+         if (tp->rx_buffers[tp->dirty_rx % RX_RING_SIZE].skb == NULL)
+		 goto oom;
+
+         /* Remove us from polling list and enable RX intr. */
+
+         napi_complete(napi);
+         iowrite32(tulip_tbl[tp->chip_id].valid_intrs, tp->base_addr+CSR7);
+
+         /* The last op happens after poll completion. Which means the following:
+          * 1. it can race with disabling irqs in irq handler
+          * 2. it can race with dise/enabling irqs in other poll threads
+          * 3. if an irq raised after beginning loop, it will be immediately
+          *    triggered here.
+          *
+          * Summarizing: the logic results in some redundant irqs both
+          * due to races in masking and due to too late acking of already
+          * processed irqs. But it must not result in losing events.
+          */
+
+         return work_done;
+
+ not_done:
+         if (tp->cur_rx - tp->dirty_rx > RX_RING_SIZE/2 ||
+             tp->rx_buffers[tp->dirty_rx % RX_RING_SIZE].skb == NULL)
+                 tulip_refill_rx(dev);
+
+         if (tp->rx_buffers[tp->dirty_rx % RX_RING_SIZE].skb == NULL)
+		 goto oom;
+
+         return work_done;
+
+ oom:    /* Executed with RX ints disabled */
+
+         /* Start timer, stop polling, but do not enable rx interrupts. */
+         mod_timer(&tp->oom_timer, jiffies+1);
+
+         /* Think: timer_pending() was an explicit signature of bug.
+          * Timer can be pending now but fired and completed
+          * before we did netif_rx_complete(). See? We would lose it. */
+
+         /* remove ourselves from the polling list */
+         napi_complete(napi);
+
+         return work_done;
+}
+
+#else /* CONFIG_TULIP_NAPI */
+
+static int tulip_rx(struct net_device *dev)
+{
+	struct tulip_private *tp = netdev_priv(dev);
+	int entry = tp->cur_rx % RX_RING_SIZE;
+	int rx_work_limit = tp->dirty_rx + (RX_RING_SIZE/SE_RD_DIV) - tp->cur_rx;
+	int received = 0;
+
+	if (tulip_debug > 4)
+		printk(KERN_DEBUG " In tulip_rx(), entry %d %8.8x.\n", entry,
+			   tp->rx_ring[entry].status);
+	/* If we own the next entry, it is a new packet. Send it up. */
+	while ( ! (tp->rx_ring[entry].status & cpu_to_le32(DescOwned))) {
+		s32 status = le32_to_cpu(tp->rx_ring[entry].status);
+
+	   /* Pilot-II : Ignore Rx Collision Error */
+  		if ((status & 0xc8c2) == 0x8040)
+			status &= ~0x8040;
+
+		if (tulip_debug > 5)
+			printk(KERN_DEBUG "%s: In tulip_rx(), entry %d %8.8x.\n",
+				   dev->name, entry, status);
+		if (--rx_work_limit < 0)
+			break;
+		if ((status & 0x38008300) != 0x0300) {
+			if ((status & 0x38000300) != 0x0300) {
+				/* Ingore earlier buffers. */
+				if ((status & 0xffff) != 0x7fff) {
+					if (tulip_debug > 1)
+						printk(KERN_WARNING "%s: Oversized Ethernet frame "
+							   "spanned multiple buffers, status %8.8x!\n",
+							   dev->name, status);
+					tp->stats.rx_length_errors++;
+				}
+			} else if (status & RxDescFatalErr) {
+				/* There was a fatal error. */
+				if (tulip_debug > 2)
+					printk(KERN_DEBUG "%s: Receive error, Rx status %8.8x.\n",
+						   dev->name, status);
+				tp->stats.rx_errors++; /* end of a packet.*/
+#if 0      
+				if (status & 0x0890) tp->stats.rx_length_errors++;
+				if (status & 0x0004) tp->stats.rx_frame_errors++;
+				if (status & 0x0002) tp->stats.rx_crc_errors++;
+				if (status & 0x0001) tp->stats.rx_fifo_errors++;
+#else
+                if (status & 0x0880) tp->stats.rx_length_errors++;
+				if (status & 0x4000) tp->stats.rx_fifo_errors++;	
+                if (status & 0x0002) tp->stats.rx_crc_errors++;
+                if (status & 0x0001) tp->stats.rx_fifo_errors++;
+                if (status & 0x0040) tp->stats.collisions++;
+#endif				
+			}
+		} else {
+			/* Omit the four octet CRC from the length. */
+			short pkt_len = ((status >> 16) & 0x7ff) - 4;
+			struct sk_buff *skb;
+
+#ifndef final_version
+			if (pkt_len > 1518) {
+				printk(KERN_WARNING "%s: Bogus packet size of %d (%#x).\n",
+					   dev->name, pkt_len, pkt_len);
+				pkt_len = 1518;
+				tp->stats.rx_length_errors++;
+			}
+#endif
+
+			/* Check if the packet is long enough to accept without copying
+			   to a minimally-sized skbuff. */
+			if (pkt_len < tulip_rx_copybreak
+				&& (skb = dev_alloc_skb(pkt_len + 2)) != NULL) {
+				skb_reserve(skb, 2);	/* 16 byte align the IP header */
+				pci_dma_sync_single_for_cpu(tp->pdev,
+							    tp->rx_buffers[entry].mapping,
+							    pkt_len, PCI_DMA_FROMDEVICE);
+#if ! defined(__alpha__)
+				skb_copy_to_linear_data(skb, tp->rx_buffers[entry].skb->data,
+						 pkt_len);
+				skb_put(skb, pkt_len);
+#else
+				memcpy(skb_put(skb, pkt_len),
+				       tp->rx_buffers[entry].skb->data,
+				       pkt_len);
+#endif
+				pci_dma_sync_single_for_device(tp->pdev,
+							       tp->rx_buffers[entry].mapping,
+							       pkt_len, PCI_DMA_FROMDEVICE);
+			} else { 	/* Pass up the skb already on the Rx ring. */
+				char *temp = skb_put(skb = tp->rx_buffers[entry].skb,
+						     pkt_len);
+
+#ifndef final_version
+				if (tp->rx_buffers[entry].mapping !=
+				    le32_to_cpu(tp->rx_ring[entry].buffer1)) {
+					printk(KERN_ERR "%s: Internal fault: The skbuff addresses "
+					       "do not match in tulip_rx: %08x vs. %Lx %p / %p.\n",
+					       dev->name,
+					       le32_to_cpu(tp->rx_ring[entry].buffer1),
+					       (long long)tp->rx_buffers[entry].mapping,
+					       skb->head, temp);
+				}
+#endif
+
+				pci_unmap_single(tp->pdev, tp->rx_buffers[entry].mapping,
+						 PKT_BUF_SZ, PCI_DMA_FROMDEVICE);
+
+				tp->rx_buffers[entry].skb = NULL;
+				tp->rx_buffers[entry].mapping = 0;
+			}
+			skb->protocol = eth_type_trans(skb, dev);
+
+			netif_rx(skb);
+
+			dev->last_rx = jiffies;
+			tp->stats.rx_packets++;
+			tp->stats.rx_bytes += pkt_len;
+		}
+		received++;
+		entry = (++tp->cur_rx) % RX_RING_SIZE;
+	}
+	return received;
+}
+#endif  /* CONFIG_TULIP_NAPI */
+
+static inline unsigned int phy_interrupt (struct net_device *dev)
+{
+#ifdef __hppa__
+	struct tulip_private *tp = netdev_priv(dev);
+	int csr12 = ioread32(tp->base_addr + CSR12) & 0xff;
+
+	if (csr12 != tp->csr12_shadow) {
+		/* ack interrupt */
+		iowrite32(csr12 | 0x02, tp->base_addr + CSR12);
+		tp->csr12_shadow = csr12;
+		/* do link change stuff */
+		spin_lock(&tp->lock);
+		tulip_check_duplex(dev);
+		spin_unlock(&tp->lock);
+		/* clear irq ack bit */
+		iowrite32(csr12 & ~0x02, tp->base_addr + CSR12);
+
+		return 1;
+	}
+#endif
+
+	return 0;
+}
+
+/* The interrupt handler does all of the Rx thread work and cleans up
+   after the Tx thread. */
+irqreturn_t tulip_interrupt(int irq, void *dev_instance)
+{
+	struct net_device *dev = (struct net_device *)dev_instance;
+	struct tulip_private *tp = netdev_priv(dev);
+	void __iomem *ioaddr = tp->base_addr;
+	int csr5;
+	int missed;
+	int rx = 0;
+	int tx = 0;
+	int oi = 0;
+	int maxrx = RX_RING_SIZE;
+	int maxtx = TX_RING_SIZE;
+	int maxoi = TX_RING_SIZE;
+#ifdef CONFIG_TULIP_NAPI
+	int rxd = 0;
+#else
+	int entry;
+#endif
+	unsigned int work_count = tulip_max_interrupt_work;
+	unsigned int handled = 0;
+ unsigned long flags = 0;
+
+	/* Let's see whether the interrupt really is for us */
+	csr5 = ioread32(ioaddr + CSR5);
+
+        if (tp->flags & HAS_PHY_IRQ)
+	        handled = phy_interrupt (dev);
+
+	if ((csr5 & (NormalIntr|AbnormalIntr)) == 0)
+		return IRQ_RETVAL(handled);
+
+	tp->nir++;
+
+	do {
+
+#ifdef CONFIG_TULIP_NAPI
+
+		if (!rxd && (csr5 & (RxIntr | RxNoBuf))) {
+			rxd++;
+			/* Mask RX intrs and add the device to poll list. */
+			iowrite32(tulip_tbl[tp->chip_id].valid_intrs&~RxPollInt, ioaddr + CSR7);
+			napi_schedule(&tp->napi);
+
+			if (!(csr5&~(AbnormalIntr|NormalIntr|RxPollInt|TPLnkPass)))
+                               break;
+		}
+
+               /* Acknowledge the interrupt sources we handle here ASAP
+                  the poll function does Rx and RxNoBuf acking */
+
+		iowrite32(csr5 & 0x0001ff3f, ioaddr + CSR5);
+
+#else
+		/* Acknowledge all of the current interrupt sources ASAP. */
+		iowrite32(csr5 & 0x0001ffff, ioaddr + CSR5);
+
+
+		if (csr5 & (RxIntr | RxNoBuf)) {
+				rx += tulip_rx(dev);
+			tulip_refill_rx(dev);
+		}
+
+#endif /*  CONFIG_TULIP_NAPI */
+
+		if (tulip_debug > 4)
+			printk(KERN_DEBUG "%s: interrupt  csr5=%#8.8x new csr5=%#8.8x.\n",
+			       dev->name, csr5, ioread32(ioaddr + CSR5));
+
+
+		if (csr5 & (TxNoBuf | TxDied | TxIntr | TimerInt)) {
+			unsigned int dirty_tx;
+
+  spin_lock_irqsave(&tp->lock,flags);			
+
+		if(tp->p_tx_coalising_en)
+		{
+	  		iowrite32(tulip_tbl[tp->chip_id].valid_intrs, ioaddr + CSR7);
+	  		iowrite32( (CSR11_TIM), (ioaddr + CSR11)); 
+	  		tp->p_tx_coalising_en=0x0;
+	  	}
+			for (dirty_tx = tp->dirty_tx; tp->cur_tx - dirty_tx > 0;
+				 dirty_tx++) {
+				int entry = dirty_tx % TX_RING_SIZE;
+				int status = le32_to_cpu(tp->tx_ring[entry].status);
+#ifdef CONFIG_PILOT_SG
+				u32 tdes1 = le32_to_cpu(tp->tx_ring[entry].length);
+#endif
+				if (status < 0)
+					break;			/* It still has not been Txed */
+
+			   /* Pilot-II  : Ignore Tx Carrier Error */
+  				if ((status & 0x8f02) == 0x8400)
+						status &= ~0x8400;
+
+				/* Check for Rx filter setup frames. */
+				if (tp->tx_buffers[entry].skb == NULL) {
+					/* test because dummy frames not mapped */
+					if (tp->tx_buffers[entry].mapping)
+						pci_unmap_single(tp->pdev,
+							 tp->tx_buffers[entry].mapping,
+							 sizeof(tp->setup_frame),
+							 PCI_DMA_TODEVICE);
+					continue;
+				}
+
+				if (status & 0x8000) {
+					/* There was an major error, log it. */
+#ifndef final_version
+					if (tulip_debug > 1)
+						printk(KERN_DEBUG "%s: Transmit error, Tx status %8.8x.\n",
+							   dev->name, status);
+#endif
+					tp->stats.tx_errors++;
+					if (status & 0x4104) tp->stats.tx_aborted_errors++;
+					if (status & 0x0C00) tp->stats.tx_carrier_errors++;
+					if (status & 0x0200) tp->stats.tx_window_errors++;
+					if (status & 0x0002) tp->stats.tx_fifo_errors++;
+					if ((status & 0x0080) && tp->full_duplex == 0)
+						tp->stats.tx_heartbeat_errors++;
+				} else {
+#ifdef CONFIG_PILOT_SG
+					tp->stats.tx_bytes += tp->tx_buffers[entry].maplen;
+#else
+					tp->stats.tx_bytes +=
+						tp->tx_buffers[entry].skb->len;
+#endif
+					tp->stats.collisions += (status >> 3) & 15;
+#ifdef CONFIG_PILOT_SG
+					if(tdes1 & (1 << 30))
+						tp->stats.tx_packets++;
+#else
+					tp->stats.tx_packets++;
+#endif
+				}
+
+#ifdef CONFIG_PILOT_SG
+				pci_unmap_single(tp->pdev, tp->tx_buffers[entry].mapping, tp->tx_buffers[entry].maplen, PCI_DMA_TODEVICE);
+#else
+				pci_unmap_single(tp->pdev, tp->tx_buffers[entry].mapping, tp->tx_buffers[entry].skb->len, PCI_DMA_TODEVICE);
+#endif
+
+				/* Free the original skb. */
+				dev_kfree_skb_irq(tp->tx_buffers[entry].skb);
+				tp->tx_buffers[entry].skb = NULL;
+				tp->tx_buffers[entry].mapping = 0;
+				tx++;
+			}
+
+#ifndef final_version
+			if (tp->cur_tx - dirty_tx > TX_RING_SIZE) {
+				printk(KERN_ERR "%s: Out-of-sync dirty pointer, %d vs. %d.\n",
+					   dev->name, dirty_tx, tp->cur_tx);
+				dirty_tx += TX_RING_SIZE;
+			}
+#endif
+
+			if (tp->cur_tx - dirty_tx < TX_RING_SIZE - 2)
+				netif_wake_queue(dev);
+
+			tp->dirty_tx = dirty_tx;
+			if (csr5 & TxDied) {
+				if (tulip_debug > 2)
+					printk(KERN_WARNING "%s: The transmitter stopped."
+						   "  CSR5 is %x, CSR6 %x, new CSR6 %x.\n",
+						   dev->name, csr5, ioread32(ioaddr + CSR6), tp->csr6);
+				tulip_restart_rxtx(tp);
+			}
+			spin_unlock_irqrestore(&tp->lock,flags);
+		}
+
+		/* Log errors. */
+		if (csr5 & AbnormalIntr) {	/* Abnormal error summary bit. */
+			if (csr5 == 0xffffffff)
+				break;
+			if (csr5 & TxJabber) tp->stats.tx_errors++;
+			if (csr5 & TxFIFOUnderflow) {
+				if ((tp->csr6 & 0xC000) != 0xC000)
+					tp->csr6 += 0x4000;	/* Bump up the Tx threshold */
+				else
+					tp->csr6 |= 0x00200000;  /* Store-n-forward. */
+				/* Restart the transmit process. */
+				tulip_restart_rxtx(tp);
+				iowrite32(0, ioaddr + CSR1);
+			}
+			if (csr5 & (RxDied | RxNoBuf)) {
+				if (tp->flags & COMET_MAC_ADDR) {
+					iowrite32(tp->mc_filter[0], ioaddr + 0xAC);
+					iowrite32(tp->mc_filter[1], ioaddr + 0xB0);
+				}
+			}
+			if (csr5 & RxDied) {		/* Missed a Rx frame. */
+                                tp->stats.rx_missed_errors += ioread32(ioaddr + CSR8) & 0xffff;
+				tp->stats.rx_errors++;
+				tulip_start_rxtx(tp);
+			}
+			/*
+			 * NB: t21142_lnk_change() does a del_timer_sync(), so be careful if this
+			 * call is ever done under the spinlock
+			 */
+			if (csr5 & (TPLnkPass | TPLnkFail | 0x08000000)) {
+				if (tp->link_change)
+					(tp->link_change)(dev, csr5);
+			}
+			if (csr5 & SystemError) {
+				int error = (csr5 >> 23) & 7;
+				/* oops, we hit a PCI error.  The code produced corresponds
+				 * to the reason:
+				 *  0 - parity error
+				 *  1 - master abort
+				 *  2 - target abort
+				 * Note that on parity error, we should do a software reset
+				 * of the chip to get it back into a sane state (according
+				 * to the 21142/3 docs that is).
+				 *   -- rmk
+				 */
+				printk(KERN_ERR "%s: (%lu) System Error occurred (%d)\n",
+					dev->name, tp->nir, error);
+			}
+			/* Clear all error sources, included undocumented ones! */
+			iowrite32(0x0800f7ba, ioaddr + CSR5);
+			oi++;
+		}
+		if (csr5 & TimerInt) {
+
+			if (tulip_debug > 2)
+				printk(KERN_ERR "%s: Re-enabling interrupts, %8.8x.\n",
+					   dev->name, csr5);
+			iowrite32(tulip_tbl[tp->chip_id].valid_intrs, ioaddr + CSR7);
+			tp->ttimer = 0;
+			oi++;
+		}
+		if (tx > maxtx || rx > maxrx || oi > maxoi) {
+			if (tulip_debug > 1)
+				printk(KERN_WARNING "%s: Too much work during an interrupt, "
+					   "csr5=0x%8.8x. (%lu) (%d,%d,%d)\n", dev->name, csr5, tp->nir, tx, rx, oi);
+
+                       /* Acknowledge all interrupt sources. */
+                        iowrite32(0x8001ffff, ioaddr + CSR5);
+                        if (tp->flags & HAS_INTR_MITIGATION) {
+                     /* Josip Loncaric at ICASE did extensive experimentation
+			to develop a good interrupt mitigation setting.*/
+                                iowrite32(0x8b240000, ioaddr + CSR11);
+#if 0
+                        } else if (tp->chip_id == LC82C168) {
+				/* the LC82C168 doesn't have a hw timer.*/
+				iowrite32(0x00, ioaddr + CSR7);
+				mod_timer(&tp->timer, RUN_AT(HZ/50));
+#endif				
+			} else {
+                 /* Mask all interrupting sources, set timer to re-enable. */
+			    iowrite32( (NormalIntr| AbnormalIntr | TimerInt), ioaddr + CSR7);
+			    iowrite32((CSR11_TIM_1), (ioaddr + CSR11));   
+			    tp->ttimer = 1;
+            }
+			break;
+		}
+
+		work_count--;
+		if (work_count == 0)
+			break;
+
+		csr5 = ioread32(ioaddr + CSR5);
+
+#ifdef CONFIG_TULIP_NAPI
+		if (rxd)
+			csr5 &= ~RxPollInt;
+	} while ((csr5 & (TxNoBuf |
+			  TxDied |
+			  TxIntr |
+			  TimerInt |
+			  /* Abnormal intr. */
+			  RxDied |
+			  TxFIFOUnderflow |
+			  TxJabber |
+			  TPLnkFail |
+			  SystemError )) != 0);
+#else
+	} while ((csr5 & (NormalIntr|AbnormalIntr)) != 0);
+
+	tulip_refill_rx(dev);
+
+	/* check if the card is in suspend mode */
+	entry = tp->dirty_rx % RX_RING_SIZE;
+	if (tp->rx_buffers[entry].skb == NULL) {
+		if (tulip_debug > 1)
+			printk(KERN_WARNING "%s: in rx suspend mode: (%lu) (tp->cur_rx = %u, ttimer = %d, rx = %d) go/stay in suspend mode\n", dev->name, tp->nir, tp->cur_rx, tp->ttimer, rx);
+		if (tp->chip_id == LC82C168) {
+			iowrite32(0x00, ioaddr + CSR7);
+			mod_timer(&tp->timer, RUN_AT(HZ/50));
+		} else {
+			if (tp->ttimer == 0 || (ioread32(ioaddr + CSR11) & 0xffff) == 0) {
+				if (tulip_debug > 1)
+					printk(KERN_WARNING "%s: in rx suspend mode: (%lu) set timer\n", dev->name, tp->nir);
+				iowrite32(tulip_tbl[tp->chip_id].valid_intrs | TimerInt,
+					ioaddr + CSR7);
+				iowrite32(TimerInt, ioaddr + CSR5);
+				iowrite32(12, ioaddr + CSR11);
+				tp->ttimer = 1;
+			}
+		}
+	}
+#endif /* CONFIG_TULIP_NAPI */
+
+	if ((missed = ioread32(ioaddr + CSR8) & 0x1ffff)) {
+		tp->stats.rx_dropped += missed & 0x10000 ? 0x10000 : missed;
+	}
+
+	if (tulip_debug > 4)
+		printk(KERN_DEBUG "%s: exiting interrupt, csr5=%#4.4x.\n",
+			   dev->name, ioread32(ioaddr + CSR5));
+
+#ifdef P3_A0
+	iowrite32(0x01, tp->base_addr + CSR2); //P3
+#endif
+ if(tp->p_tx_coalising_en)
+   {
+     iowrite32((CSR11_NTP), (ioaddr + CSR11));   
+   }
+ else
+   {
+     if(!tp->ttimer) //timer is alreasy programmed to smaller value
+       //reload the timer,we want to get timer interrupts only when there is no rx/tx
+       iowrite32((CSR11_TIM), (ioaddr + CSR11));   
+   }
+	return IRQ_HANDLED;
+}