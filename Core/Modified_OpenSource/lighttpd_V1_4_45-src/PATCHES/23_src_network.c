--- lighttpd/src/network.c	2019-12-19 02:58:54.266100264 -0500
+++ lighttpd_new/src/network.c	2019-12-30 05:04:23.327780429 -0500
@@ -1294,15 +1294,14 @@
 					return -1;
 				} 
 				
-				if(g_corefeatures.ethernet_over_usb_support == ENABLED &&
-	           		strncmp(g_serviceconf.InterfaceName,"both",strlen("both")) != 0 &&
-			   		strncmp(InterfaceName,"FFFFFFFFFFFFFFFF",MAX_SERVICE_IFACE_NAME_SIZE) != 0 )	
+			}
+			if(g_corefeatures.ethernet_over_usb_support == ENABLED &&
+	           	strncmp(g_serviceconf.InterfaceName,"both",strlen("both")) != 0 &&
+				strncmp(InterfaceName,"FFFFFFFFFFFFFFFF",MAX_SERVICE_IFACE_NAME_SIZE) != 0 )	
+			{
+				if (0 != network_server_init(srv, dc->string, s,"usb0")) 
 				{
-					if (0 != network_server_init(srv, dc->string, s,"usb0")) 
-					{
-						buffer_free(b);
-						return -1;
-					}
+					return -1;
 				}
 			}
 		}
