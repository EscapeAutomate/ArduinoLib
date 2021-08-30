#ifndef EscLogs_h
#define EscLogs_h

#pragma once

#define ESC_LOGERROR(x)        if(_ES_LOGLEVEL_>0) { Serial.print("[ES ERROR] "); Serial.println(x); }
#define ESC_LOGERROR1(x,y)     if(_ES_LOGLEVEL_>0) { Serial.print("[ES ERROR] "); Serial.print(x); Serial.print(" "); Serial.println(y); }
							   	  									
#define ESC_LOGWARN(x)         if(_ES_LOGLEVEL_>1) { Serial.print("[ES WARN] "); Serial.println(x); }
#define ESC_LOGWARN1(x,y)      if(_ES_LOGLEVEL_>1) { Serial.print("[ES WARN] "); Serial.print(x); Serial.print(" "); Serial.println(y); }
							   	  									
#define ESC_LOGINFO(x)         if(_ES_LOGLEVEL_>2) { Serial.print("[ES INFO] "); Serial.println(x); }
#define ESC_LOGINFO1(x,y)      if(_ES_LOGLEVEL_>2) { Serial.print("[ES INFO] "); Serial.print(x); Serial.print(" "); Serial.println(y); }
#define ESC_LOGINFO2(x,y,z)    if(_ES_LOGLEVEL_>2) { Serial.print("[ES INFO] "); Serial.print(x); Serial.print(" "); Serial.print(y); Serial.print(" "); Serial.println(z); }
#define ESC_LOGINFO3(x,y,z,w)  if(_ES_LOGLEVEL_>2) { Serial.print("[ES INFO] "); Serial.print(x); Serial.print(" "); Serial.print(y); Serial.print(" "); Serial.print(z); Serial.print(" "); Serial.println(w); }
								  									
#define ESC_LOGDEBUG(x)		   if(_ES_LOGLEVEL_>3) { Serial.print("[ES DEBUG] "); Serial.println(x); }
#define ESC_LOGDEBUG1(x,y)	   if(_ES_LOGLEVEL_>3) { Serial.print("[ES DEBUG] "); Serial.print(x); Serial.print(" "); Serial.println(y); }
#define ESC_LOGDEBUG2(x,y,z)   if(_ES_LOGLEVEL_>3) { Serial.print("[ES DEBUG] "); Serial.print(x); Serial.print(" "); Serial.print(y); Serial.print(" "); Serial.println(z); }
#define ESC_LOGDEBUG3(x,y,z,w) if(_ES_LOGLEVEL_>3) { Serial.print("[ES DEBUG] "); Serial.print(x); Serial.print(" "); Serial.print(y); Serial.print(" "); Serial.print(z); Serial.print(" "); Serial.println(w); }


#endif    //EscLogs_h