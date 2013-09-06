
#ifndef _URL_H_
#define _URL_H_ 1

/**
 * Dependencies
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


/**
 * Max length of a url protocol scheme
 */

#define URL_PROTOCOL_MAX_LENGTH 16


/**
 * Max length of a url host part
 */

#define URL_HOSTNAME_MAX_LENGTH 128


/**
 * Max length of a url tld part
 */

#define URL_TLD_MAX_LENGTH 16


/**
 * Max length of a url auth part
 */

#define URL_AUTH_MAX_LENGTH 32


/**
 * URI Schemes
 * http://en.wikipedia.org/wiki/URI_scheme
 */

char *URL_SCHEMES[] = {
  // official IANA registered schemes
  "aaa", "aaas", "about", "acap", "acct", "adiumxtra", "afp", "afs", "aim", "apt", "attachment", "aw",
  "beshare", "bitcoin", "bolo", "callto", "cap", "chrome", "crome-extension", "com-evenbrite-attendee",
  "cid", "coap", "coaps","content", "crid", "cvs", "data", "dav", "dict", "lna-playsingle", "dln-playcontainer",
  "dns", "dtn", "dvb", "ed2k", "facetime", "fax", "feed", "file", "finger", "fish","ftp", "geo", "gg","git",
  "gizmoproject", "go", "gopher", "gtalk", "h323", "hcp", "http", "https", "iax", "icap", "icon","im",
  "imap", "info", "ipn", "ipp", "irc", "irc6", "ircs", "iris", "iris.beep", "iris.xpc", "iris.xpcs","iris.lws",
  "itms", "jabber", "jar", "jms", "keyparc", "lastfm", "ldap", "ldaps", "magnet", "mailserver","mailto",
  "maps", "market", "message", "mid", "mms", "modem", "ms-help", "mssettings-power", "msnim", "msrp",
  "msrps", "mtqp", "mumble", "mupdate", "mvn", "news", "nfs", "ni", "nih", "nntp", "notes","oid",
  "paquelocktoken", "pack", "palm", "paparazzi", "pkcs11", "platform", "pop", "pres", "prospero", "proxy",
  "psyc","query", "reload", "res", "resource", "rmi", "rsync", "rtmp","rtsp",  "secondlife", "service","session",
  "sftp", "sgn", "shttp", "sieve", "sip", "sips", "skype", "smb", "sms", "snews", "snmp", "soap.beep","soap.beeps",
  "soldat", "spotify", "ssh", "steam", "svn", "tag", "teamspeak", "tel", "telnet", "tftp", "things","thismessage",
  "tn3270", "tip", "tv", "udp", "unreal", "urn", "ut2004", "vemmi","ventrilo", "videotex", "view-source", "wais","webcal",
  "ws", "wss", "wtai", "wyciwyg", "xcon", "xcon-userid", "xfire","xmlrpc.beep",  "xmlrpc.beeps", "xmpp", "xri","ymsgr",

  // unofficial schemes
  "javascript", "jdbc", "doi"
};


/**
 * `url_data` struct that defines parts
 * of a parsed URL such as host and protocol
 */

typedef struct url_data {
  char *href;
  char *protocol;
  char *host;
  char *auth;
  char *hostname;
  char *pathname;
  char *search;
  char *path;
  char *hash;
  char *query;
  char *port;
} url_data_t;


// prototype

/**
 * Parses a url into parts and returns
 * a `url_data_t *` pointer
 */

url_data_t *
url_parse (char *url);

char *
url_get_protocol (char *url);

char *
url_get_auth (char *url);

char *
url_get_hostname (char *url);

char *
url_get_host (char *url);

char *
url_get_pathname (char *url);

char *
url_get_path (char *url);

char *
url_get_search (char *url);

char *
url_get_query (char *url);

char *
url_get_hash (char *url);

char *
url_get_port (char *url);

char *
url_format (url_data_t *data);

void
url_free (url_data_t *data);

bool
url_is_protocol (char *str);

bool
url_has_auth (char *str);

void
url_inspect (char *url);

void
url_data_inspect (url_data_t *data);

void
url_free (url_data_t *data);


// implementation

static char *
strff (char *ptr, int n) {
  int y = 0;
  for (int i = 0; i < n; ++i) {
    y = *ptr++;
  }

  return strdup(ptr);
}

static char *
strrwd (char *ptr, int n) {
  int y = 0;
  for (int i = 0; i < n; ++i) {
    y = *ptr--;
  }

  return strdup(ptr);
}

static char *
get_part (char *url, const char *format, int l) {
  bool has = false;
  char *tmp = malloc(sizeof(char));
  char *tmp_url = strdup(url);
  char *fmt_url = strdup(url);
  char *ret = malloc(sizeof(char));

  if (!tmp || !tmp_url || !fmt_url || !ret)
    return NULL;

  strcpy(tmp, "");
  strcpy(fmt_url, "");

  // move pointer exactly the amount
  // of characters in the `prototcol` char
  // plus 3 characters that represent the `://`
  // part of the url
  fmt_url = strff(fmt_url, l);

  sscanf(fmt_url, format, tmp);

  if (0 != strcmp(tmp, tmp_url)) {
    has = true;
    ret = strdup(tmp);
  }

  // descrement pointer to original
  // position so it can be free'd
  fmt_url = strrwd(fmt_url, l);

  free(tmp);
  free(tmp_url);
  free(fmt_url);

  return has? ret : NULL;
}

url_data_t *
url_parse (char *url) {
  url_data_t *data = malloc(sizeof(url_data_t));
  if (!data) return NULL;

  data->href = url;
  char *tmp;
  char *tmp_url = strdup(url);

  char *protocol = url_get_protocol(tmp_url);
  if (!protocol) return NULL;
  // length of protocol plus ://
  int protocol_len = (int) strlen(protocol) + 3;
  data->protocol = protocol;

  char *auth = malloc(sizeof(char));
  int auth_len = 0;
  if ((tmp = strstr(tmp_url, "@"))) {
    auth = get_part(tmp_url, "%[^@]", protocol_len);
    auth_len = strlen(auth);
    if (auth) auth_len++;
  }

  data->auth = auth;

  char *hostname = get_part(tmp_url, "%[^/]", protocol_len + auth_len);
  if (!hostname) return NULL;
  int hostname_len = (int) strlen(hostname);
  char *tmp_hostname = strdup(hostname);
  data->hostname = hostname;

  char *host = malloc(strlen(tmp_hostname) * sizeof(char));
  sscanf(tmp_hostname, "%[^:]", host);
  if (!host) return NULL;
  int host_len = (int) strlen(host);
  data->host = host;

  char *path = malloc(sizeof(char));
  char *tmp_path;
  if (!path) return NULL;
  tmp_path = get_part(tmp_url, "/%s", protocol_len + auth_len + hostname_len);
  path = realloc(path, strlen(tmp_path) * sizeof(char));
  sprintf(path, "/%s", tmp_path);
  data->path = path;
  free(tmp_path);

  char *pathname = malloc(sizeof(char));
  if (!pathname) return NULL;
  strcat(pathname, "");
  tmp_path = strdup(path);
  sscanf(tmp_path, "%[^? | ^#]", pathname);
  int pathname_len = strlen(pathname);
  data->pathname = pathname;

  char *search = malloc(sizeof(search));
  if (!search) return NULL;
  tmp_path = strff(path, pathname_len);
  strcat(search, "");
  sscanf(tmp_path, "%[^#]", search);
  data->search = search;
  int search_len = strlen(search);
  tmp_path = strrwd(tmp_path, pathname_len);
  free(tmp_path);

  char *query = malloc(sizeof(char));
  if (!query) return NULL;
  sscanf(search, "?%s", query);
  data->query = query;

  char *hash = malloc(sizeof(char));
  if (!hash) return NULL;
  tmp_path = strff(path, pathname_len + search_len);
  strcat(hash, "");
  sscanf(tmp_path, "%s", hash);
  data->hash = hash;
  tmp_path = strrwd(tmp_path, pathname_len + search_len);
  free(tmp_path);

  char *port = malloc(sizeof(char));
  if (!port) return NULL;

  tmp_hostname = strff(hostname, host_len + 1); // +1 for ':' char
  sscanf(tmp_hostname, "%s", port);
  data->port = port;
  tmp_hostname = strrwd(tmp_hostname, host_len + 1);
  free(tmp_hostname);

  return data;
}

bool
url_is_protocol (char *str) {
  int count = sizeof(URL_SCHEMES) / sizeof(URL_SCHEMES[0]);

  for (int i = 0; i < count; ++i) {
    if (0 == strcmp(URL_SCHEMES[i], str)) {
      return true;
    }
  }

  return false;
}

char *
url_get_protocol (char *url) {
  char *protocol = malloc(URL_PROTOCOL_MAX_LENGTH * sizeof(char));
  if (!protocol) return NULL;
  sscanf(url, "%[^://]", protocol);
  if (url_is_protocol(protocol)) return protocol;
  return NULL;
}


char *
url_get_auth (char *url) {
  char *protocol = url_get_protocol(url);
  if (!protocol) return NULL;
  int l = (int) strlen(protocol) + 3;
  return get_part(url, "%[^@]", l);
}

char *
url_get_hostname (char *url) {
  int l = 3;
  char *protocol = url_get_protocol(url);
  char *auth = url_get_auth(url);

  if (!protocol) return NULL;
  if (auth) l += strlen(auth) + 1; // add one @ symbol
  if (auth) free(auth);

  l += (int) strlen(protocol);

  free(protocol);

  return get_part(url, "%[^/]", l);
}

char *
url_get_host (char *url) {
  char *host = malloc(sizeof(char));
  char *hostname = url_get_hostname(url);

  if (!host || !hostname) return NULL;

  sscanf(hostname, "%[^:]", host);

  free(hostname);

  return host;
}

char *
url_get_pathname (char *url) {
  char *path = url_get_path(url);
  char *pathname = malloc(sizeof(char));

  if (!path || !pathname) return NULL;

  strcat(pathname, "");
  sscanf(path, "%[^?]", pathname);

  free(path);

  return pathname;
}

char *
url_get_path (char *url) {
  int l = 3;
  char *tmp_path;
  char *path = malloc(sizeof(char));
  char *protocol = url_get_protocol(url);
  char *auth = url_get_auth(url);
  char *hostname = url_get_hostname(url);

  if (!protocol || !hostname)
    return NULL;

  l += (int) strlen(protocol);
  l += (int) strlen(hostname);

  if (auth) l+= (int) strlen(auth) +1; // @ symbol

  tmp_path = get_part(url, "/%s", l);
  path = realloc(path, strlen(tmp_path) * sizeof(char));
  sprintf(path, "/%s", tmp_path);

  free(protocol);
  if (auth) free(auth);
  free(hostname);
  free(tmp_path);

  return path;

}

char *
url_get_search (char *url) {
  char *path = url_get_path(url);
  char *pathname = url_get_pathname(url);
  char *search = malloc(sizeof(char));

  if (!path || !search) return NULL;

  char *tmp_path = strff(path, (int)strlen(pathname));
  strcat(search, "");
  sscanf(tmp_path, "%[^#]", search);

  tmp_path = strrwd(tmp_path, (int)strlen(pathname));

  free(path);
  free(pathname);

  return search;
}

char *
url_get_query (char *url) {
  char *search = url_get_search(url);
  char *query = malloc(sizeof(char));
  if (!search) return NULL;
  sscanf(search, "?%s", query);
  free(search);
  return query;
}

char *
url_get_hash (char *url) {
  char *path = url_get_path(url);
  char *pathname = url_get_pathname(url);
  char *search = url_get_search(url);
  char *hash = malloc(sizeof(char));

  if (!path || !pathname || !hash)
    return NULL;

  char *tmp_path = strff(path, (int)strlen(pathname) + (int)strlen(search));
  strcat(hash, "");
  sscanf(tmp_path, "%s", hash);

  free(path);
  free(tmp_path);
  free(pathname);
  if (search) free(search);

  return hash;
}

char *
url_get_port (char *url) {
  char *port = malloc(sizeof(char));
  char *hostname = url_get_hostname(url);
  char *host = url_get_host(url);
  if (!port || !hostname) return NULL;

  char *tmp_hostname = strff(hostname, strlen(host) +1);
  sscanf(tmp_hostname, "%s", port);

  free(hostname);
  free(tmp_hostname);
  return port;
}

void
url_inspect (char *url) {
  url_data_inspect(url_parse(url));
}


void
url_data_inspect (url_data_t *data) {
  printf("#url =>\n");
  printf("    .href: \"%s\"\n",     data->href);
  printf("    .protocol: \"%s\"\n", data->protocol);
  printf("    .host: \"%s\"\n",     data->host);
  printf("    .auth: \"%s\"\n",     data->auth);
  printf("    .hostname: \"%s\"\n", data->hostname);
  printf("    .pathname: \"%s\"\n", data->pathname);
  printf("    .search: \"%s\"\n",   data->search);
  printf("    .path: \"%s\"\n",     data->path);
  printf("    .hash: \"%s\"\n",     data->hash);
  printf("    .query: \"%s\"\n",    data->query);
  printf("    .port: \"%s\"\n",     data->port);
}

void
url_free (url_data_t *data) {
  if (!data) return;
  if (data->auth) free(data->auth);
  if (data->protocol) free(data->protocol);
  if (data->hostname) free(data->hostname);
  if (data->host) free(data->host);
  if (data->pathname) free(data->pathname);
  if (data->path) free(data->path);
  if (data->hash) free(data->hash);
  if (data->search) free(data->search);
  if (data->query) free(data->query);
}

char *
url_format (url_data_t *data) {
  if (!data) return NULL;
  char *url = strdup(data->href);
  if (!data->pathname) {
    sprintf(url, "%s/", url);
  }

  return url;
}


#endif