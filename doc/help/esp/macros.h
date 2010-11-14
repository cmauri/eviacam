#ifdef _FORMAT_SINGLE_FILE
#define _HREF(topicfile,linkedtext) \
  <a href=\"_HELP_FILE#topicfile\">linkedtext</a>
#else
#define _HREF(topicfile,linkedtext) \
  <a href=\"topicfile.htm\">linkedtext</a>
#endif

