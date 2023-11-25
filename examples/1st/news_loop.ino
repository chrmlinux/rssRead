

//==========================================================
//
// rssRead
// Read rss (xml) and return an array of strings.
//
// date/author    : 2022/07/13 @chrmlinux03
// update/author  : 2022/07/14 @chrmlinux03
// LICENSE        : MIT
//
//==========================================================

#include "rssRead.hpp"
static rssRead rss;
//const char *url  = "https://news.yahoo.co.jp/rss/topics/top-picks.xml";
const char *url = "https://www.servizitelevideo.rai.it/televideo/pub/rss101.xml";
//const char *url = "https://www.asahi.com/rss/asahi/newsheadlines.rdf";
//const char *url = "https://mainichi.jp/rss/etc/mainichi-flash.rss";
//const char *url = "https://news.google.com/rss/search?q=Japan&hl=ja&gl=JP&ceid=JP:ja";

//-------------------------------------------------
// WiFiConnect
//-------------------------------------------------
const char *ssid = "TIM-55267293";
const char *pswd = "U4RuRf5kybXu6eUbcyu5bchZ";

void WiFiConnect(void) {

  WiFi.disconnect(true);
  WiFi.begin(ssid, pswd);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();

}

//-------------------------------------------------
// setup
//-------------------------------------------------
void setup(void) {

  Serial.begin(115200); while (!Serial);
  WiFiConnect();
  rss.begin();
  
}

//-------------------------------------------------
// loop
//-------------------------------------------------
void loop(void) {

  rss.axs(url);
  //il primo tag restituisce la scritta "Televideo Rai" ecc.
  rss.finds(String("description"));
  //il secondo mi dà l'ultima notizia
  String dst = rss.finds(String("description"));
  dst.replace("<![CDATA[", "");
  dst.replace("]]>", "");
  Serial.printf("%s\n", dst.c_str());
  
  delay(2000);

}
