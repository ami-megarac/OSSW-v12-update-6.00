--- lighttpd/src/mod_proxy.c	2017-01-14 13:05:17.000000000 +0800
+++ lighttpd_new/src/mod_proxy.c	2019-10-09 13:50:26.837482302 +0800
@@ -690,7 +690,20 @@
 		proxy_set_header(con, "X-Host", con->request.http_host->ptr);
 	}
 	proxy_set_header(con, "X-Forwarded-Proto", con->uri.scheme->ptr);
-
+#ifdef CONFIG_SPX_FEATURE_REDFISH_ENABLED
+	data_string *key_ds = (data_string *)array_get_element(hctx->remote_conn->environment, "SSL_CLIENT_CERT");
+	if (key_ds != NULL) {
+		for (char* p = key_ds->value->ptr; p = strchr(p, '\n'); ++p) {
+			*p = '\\';
+		}
+		proxy_set_header(con, "X-Client-Certificate", key_ds->value->ptr);
+	}
+	
+	key_ds = (data_string *)array_get_element(hctx->remote_conn->environment, "REMOTE_USER");
+	if (key_ds != NULL) {
+		proxy_set_header(con, "X-Username", key_ds->value->ptr);
+	}
+#endif
 	/* request header */
 	for (i = 0; i < con->request.headers->used; i++) {
 		data_string *ds;
