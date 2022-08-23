--- lighttpd-1.4.45/src/mod_redirect.c	2017-01-14 13:05:17.000000000 +0800
+++ lighttpd_V1_4_45-6.17.0.0.0-src/data/.workspace/lighttpd/src/mod_redirect.c	2020-05-11 16:57:15.660891000 +0800
@@ -172,6 +172,33 @@
 	return 0;
 }
 #endif
+
+int get_port_https (server *srv, int *port)
+{
+	int i=0;
+	for (i = 1; i < srv->config_context->used; i++)
+	{
+		data_config *dc = (data_config *)srv->config_context->data[i];
+		specific_config *s = srv->config_storage[i];
+		size_t j;
+
+		/* not our stage */
+		if (COMP_SERVER_SOCKET != dc->comp)
+			continue;
+		/* check if we already know this socket,
+		* if yes, don't init it */
+		for (j = 0; j < srv->srv_sockets.used; j++)
+		{
+			if (buffer_is_equal(srv->srv_sockets.ptr[j]->srv_token, dc->string))
+			{
+				break;	
+			}
+		}
+		*port = strtol (dc->string->ptr+1,NULL,10);
+	}
+	return 0;
+}
+
 static handler_t mod_redirect_uri_handler(server *srv, connection *con, void *p_data) {
 #ifdef HAVE_PCRE_H
 	plugin_data *p = p_data;
@@ -194,12 +221,35 @@
 		const char *pattern;
 		size_t pattern_len;
 		int n;
+		int port =0;
 		pcre_keyvalue *kv = p->conf.redirect->kv[i];
 # define N 10
 		int ovec[N * 3];
 
 		match       = kv->key;
 		extra       = kv->key_extra;
+		
+		get_port_https (srv, &port);
+
+		if(port != 443) {
+			buffer_reset(kv->value);
+			buffer_copy_string(kv->value, "https://%1:");
+			buffer_append_int(kv->value, port);
+			buffer_append_string(kv->value,"$1");
+		}
+		if(srv->srvconf.port == 80 && port != 443) {
+			buffer_reset(kv->value);
+			buffer_copy_string(kv->value, "https://%0:");
+			buffer_append_int(kv->value, port);
+			buffer_append_string(kv->value,"$0");
+		}
+		if(srv->srvconf.port == 80 && port == 443) {
+			buffer_reset(kv->value);
+			buffer_copy_string(kv->value, "https://%0:");
+			buffer_append_int(kv->value, 443);
+			buffer_append_string(kv->value,"$0");
+		}
+
 		pattern     = kv->value->ptr;
 		pattern_len = buffer_string_length(kv->value);
 
