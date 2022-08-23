--- lighttpd-1.4.45/src/buffer.c	2017-01-14 00:05:17.000000000 -0500
+++ lighttpd_old/src/buffer.c	2021-12-24 01:38:47.429397520 -0500
@@ -46,6 +46,8 @@
 
 	free(b->ptr);
 	free(b);
+	if( b != NULL)
+		b = NULL;
 }
 
 void buffer_reset(buffer *b) {
@@ -201,6 +203,40 @@
 void buffer_append_string(buffer *b, const char *s) {
 	buffer_append_string_len(b, s, NULL != s ? strlen(s) : 0);
 }
+int buffer_prepare_append(buffer *b, size_t size) {
+    if (!b) return -1;
+
+    if (0 == b->size) {
+        b->size = size;
+
+        /* always allocate a multiply of BUFFER_PIECE_SIZE */
+        b->size += BUFFER_PIECE_SIZE - (b->size % BUFFER_PIECE_SIZE);
+
+        b->ptr = malloc(b->size);
+        b->used = 0;
+        force_assert(b->ptr);
+    } else if (b->used + size > b->size) {
+        b->size += size;
+
+        /* always allocate a multiply of BUFFER_PIECE_SIZE */
+        b->size += BUFFER_PIECE_SIZE - (b->size % BUFFER_PIECE_SIZE);
+
+        b->ptr = realloc(b->ptr, b->size);
+        force_assert(b->ptr);
+    }
+    return 0;
+}
+
+int buffer_append_memory(buffer *b, const char *s, size_t s_len) {
+    if (!s || !b) return -1;
+    if (s_len == 0) return 0;
+
+    buffer_prepare_append(b, s_len);
+    memcpy(b->ptr + b->used, s, s_len);
+    b->used += s_len;
+
+    return 0;
+}
 
 /**
  * append a string to the end of the buffer
@@ -1097,5 +1133,6 @@
 	fprintf(stderr, "%s.%u: %s\n", filename, line, msg);
 	print_backtrace(stderr);
 	fflush(stderr);
-	abort();
+	// on EBADF lighttpd crashes. Don't abort on socket EBADF
+	//abort();
 }
