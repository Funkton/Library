//"awtools.ulp" - include file containing useful procedures and functions
//(C) 2011-06-14 Andreas Weidner

#require 5.0303
//Earlier EAGLE versions (from 5.0 upwards) MAY be able to run these functions,
//but were not properly tested

#usage "en:---<br>\n"
    "<font color=red><b>Include-File:</b></font> "
    "This file is used by several programs and cannot be run on its own.",
  "de:---<br>\n"
    "<font color=red><b>Include-File:</b></font> "
    "Diese Datei wird von diversen Programmen verwendet und ist nicht "
    "selbständig lauffähig."

//General definitions
enum {dlgSpace=6};

string AWExitCommand="";

//PROCEDURES FOR STRING MANIPULATION ------------------------------------------

string AWTrim(string Text) {
  //Removes leading and trailing blanks, tabs and line feeds from TEXT
  string NewText,Character;
  NewText=Text;
  //Remove leading blanks and tabs
  Character=strsub(NewText,0,1);
  while ((Character==" ") || (Character=="\t") || (Character=="\n")) {
    NewText=strsub(NewText,1);
    Character=strsub(NewText,0,1);
    }
  //Ditto with trailing ones
  Character=strsub(NewText,strlen(NewText)-1);
  while ((Character==" ") || (Character=="\t") || (Character=="\n")) {
    NewText=strsub(NewText,0,strlen(NewText)-1);
    Character=strsub(NewText,strlen(NewText)-1);
    }
  return NewText;
  }

string AWReplace(string Text,string From,string To) {
  //Replaces all (case-sensitive) occurrences of the string FROM in the string
  //TEXT by the string TO and returns the resulting string
  int Pos;
  Pos=strstr(Text,From);
  while (Pos!=-1) {
    Text=strsub(Text,0,Pos)+To+strsub(Text,Pos+strlen(From));
    Pos=strstr(Text,From,Pos+strlen(To));
    }
  return Text;
  }

string AWReplaceText(string Text,string From,string To) {
  //Replaces all (case-insensitive) occurrences of the string FROM in the
  //string TEXT by the string TO and returns the resulting string
  string LowerText;
  int Pos;
  //Use the lowercase strings for searching, but return the contents of the
  //original string
  From=strlwr(From);
  LowerText=strlwr(Text);
  Pos=strstr(LowerText,From);
  while (Pos!=-1) {
    Text=strsub(Text,0,Pos)+To+strsub(Text,Pos+strlen(From));
    LowerText=strsub(LowerText,0,Pos)+To+strsub(LowerText,Pos+strlen(From));
    Pos=strstr(LowerText,From,Pos+strlen(To));
    }
  return Text;
  }

string AWReplaceVal(string Text,string From,int To) {
  //Replaces all (case-sensitive) occurrences of the string FROM in the string
  //TEXT by the character with the EAGLE character code TO
  int Pos;
  //Force the character code into the range 0..255
  To=max(min(To,255),0);
  Pos=strstr(Text,From);
  while (Pos!=-1) {
    Text=strsub(Text,0,Pos)+"?"+strsub(Text,Pos+strlen(From));
    Text[Pos]=To;
    Pos=strstr(Text,From,Pos+1);
    }
  return Text;
  }

string AWText2Script(string Text,int Backslashes,int ExclamationMarks) {
  //Converts the string TEXT so that it can be used in a script (each single
  //quote replaced by two single quotes). If BACKSLASHES!=0, each single
  //backslash is replaced by two backslashes. If EXCLAMATIONMARKS!=0, each
  //exclamation mark is escaped with a backslash)
  int Pos;
  //Double all single quotes
  Pos=strstr(Text,"'");
  while (Pos!=-1) {
    Text=strsub(Text,0,Pos+1)+strsub(Text,Pos);
    Pos=strstr(Text,"'",Pos+2);
    }
  //Double all backslashes, if desired
  if (Backslashes) {
    Pos=strstr(Text,"\\");
    while (Pos!=-1) {
      Text=strsub(Text,0,Pos+1)+strsub(Text,Pos);
      Pos=strstr(Text,"\\",Pos+2);
      }
    }
  //Escape exclamation marks, if desired
  if (ExclamationMarks) {
    Pos=strstr(Text,"!");
    while (Pos!=-1) {
      Text=strsub(Text,0,Pos)+"\\"+strsub(Text,Pos);
      Pos=strstr(Text,"!",Pos+2);
      }
    }
  return Text;
  }

string AWHTMLText(string Text,int Backslashes) {
  //Converts the TEXT so that it can be displayed in an HTML display context
  //(e.g., replaces '<' by '&lt;' etc. If BACKSLASHES!=0, backslashes are
  //converted to '\\'
  string Result;
  Result=AWReplace(AWReplace(AWReplace(Text,"<","&lt;"),">","&gt;"),"\n",
    "<br>\n");
  if (Backslashes)
    Result=AWReplace(Result,"\\","\\\\");
  return Result;
  }

string AWEscapeString(string Text,int EscapeBars,int EscapeEquals) {
  //Escapes special characters of the string TEXT with a backslash: '\' is
  //changed to '\\', a linefeed to '\n' and a tab to '\t'. Additionally, if
  //ESCAPEBARS!=0, '|' is changed to '\b', and if ESCAPEEQUALS!=0, '=' is
  //changed to '\e'
  Text=AWReplace(AWReplace(AWReplace(Text,"\\","\\\\"),"\n","\\n"),"\t","\\t");
  if (EscapeBars)
    Text=AWReplace(Text,"|","\\b");
  if (EscapeEquals)
    Text=AWReplace(Text,"=","\\e");
  return Text;
  }

string AWUnescapeString(string Text) {
  //Searches the string TEXT for characters escaped with the previous function
  //and replaces the escape sequences with the original characters. If an
  //invalid escape sequence is found, a question mark is inserted
  int Pos;
  string Char;
  Pos=strstr(Text,"\\");
  while (Pos>-1) {
    Char=strsub(Text,Pos+1,1);
    if (Char=="n")
      Text=strsub(Text,0,Pos)+"\n"+strsub(Text,Pos+2);
    else if (Char=="b")
      Text=strsub(Text,0,Pos)+"|"+strsub(Text,Pos+2);
    else if (Char=="e")
      Text=strsub(Text,0,Pos)+"="+strsub(Text,Pos+2);
    else if (Char=="t")
      Text=strsub(Text,0,Pos)+"\t"+strsub(Text,Pos+2);
    else if (Char=="\\")
      Text=strsub(Text,0,Pos+1)+strsub(Text,Pos+2);
    else
      Text=strsub(Text,0,Pos)+"?"+strsub(Text,Pos+2);
    Pos=strstr(Text,"\\",Pos+1);
    }
  return Text;
  }

string AWTodayText() {
  //Returns the current date in international form (YYYY-MM-DD)
  return t2string(time(),"yyyy-MM-dd");
  }

string AWNowText(int Seconds) {
  //Returns the current date and time in international form, either with
  //(SECONDS!=0) or without (SECONDS=0) seconds (YYYY-MM-DD HH:MM:SS)
  if (Seconds)
    return t2string(time(),"yyyy-MM-dd, hh:mm:ss");
  return t2string(time(),"yyyy-MM-dd, hh:mm");
  }

string AWBoolString(int Bool) {
  //Depending on the Boolean value BOOL, returns either '0' or '1'
  if (Bool)
    return "1";
  else
    return "0";
  }

//PROCEDURES FOR STRING LOCALISATION ------------------------------------------

int AWIsGerman() {
  //Returns 1, if a German EAGLE version is running (0 otherwise)
  if (language()!="de")
    return 1;
  return 0;
  }

string AWLocalise(string EnglishText,string GermanText) {
  //Depending on the current EAGLE language, returns ENGLISHTEXT or GERMANTEXT
  //and replaces HTML syntax by the proper EAGLE characters
  string Text;
  //Select the desired language string
  if (AWIsGerman())
    Text=GermanText;
  else
    Text=EnglishText;
  //Replace HTML syntax (not necessary anymore, because EAGLE can now cope with
  //Unicode, but just in case and to be backward-compatible)
  Text=AWReplaceVal(Text,"&auml;",228);
  Text=AWReplaceVal(Text,"&Auml;",196);
  Text=AWReplaceVal(Text,"&ouml;",246);
  Text=AWReplaceVal(Text,"&Ouml;",214);
  Text=AWReplaceVal(Text,"&uuml;",252);
  Text=AWReplaceVal(Text,"&Uuml;",220);
  Text=AWReplaceVal(Text,"&szlig;",223);
  Text=AWReplaceVal(Text,"&micro;",181);
  Text=AWReplaceVal(Text,"&deg;",176);
  Text=AWReplaceVal(Text,"&plusmn;",177);
  return Text;
  }

void AWStatus(string EnglishText,string GermanText) {
  //Depending on the current EAGLE language, displays either ENGLISHTEXT or
  //GERMANTEXT in the status bar
  status(AWLocalise(EnglishText,GermanText));
  }

//PROCEDURES FOR GENERAL FILE ACCESS -----------------------------------------

int AWDirEntryExists(string Name) {
  //Returns 1 if the file (or directory) with the NAME exists (0 otherwise)
  string Files[];
  int Nr;
  //Throw away any trailing slash
  if (strsub(Name,strlen(Name)-1)=="/")
    Name=strsub(Name,0,strlen(Name)-1);
  //Check, if the directory entry exists
  Nr=fileglob(Files,Name);
  if (Nr==0)
    return 0;
  return 1;
  }

string AWEAGLEPath() {
  //Returns the base path of the EAGLE installation (this is assumed to be the
  //parent directory of the ULP file itself) including directory separator
  string FileName;
  FileName=filedir(argv[0]);
  return filedir(strsub(FileName,0,strlen(FileName)-1));
  }

string AWGetULPSettingsPath(int ShowWarning) {
  //Returns the complete name of a special folder for ULP settings. Previously,
  //ULP settings were stored in the ULP's directory. Unfortunately, if EAGLE
  //resides on a server, this is undesired, so another location for storing is
  //necessary. Therefore, the settings path is defined as having the name
  //'settings' and residing parallel to one of the given project paths. If such
  //a path does NOT exist and SHOWWARNING=1, then the user is asked to create
  //the folder manually or cancel the ULP. Returns an empty string if still
  //no such folder is available. If SHOWWARNING=2, the user cannot cancel the
  //dialog, but MUST create the directory properly
  int Nr,Pos,Count,Result=0;
  string Path,Warning;
  //Repeat the search for a settings folder until one is available or the user
  //cancels the operation
  while (Result==0) {
    Count=0;
    Warning="";
    //Go through all of EAGLE's project paths
    for (Nr=0;Nr<16;Nr++) {
      Path=path_epf[Nr];
      if (Path=="")
        continue;
      //Ignore the project path when it contains the import/export autoscripts
      Pos=strstr(Path,"autoscripts");
      if (Pos>=0)
        continue;
      //Ignore the path if it is not a subdirectory of another path
      Pos=strrstr(Path,"/");
      if (Pos<0)
        continue;
      //Replace the last project path subdirectory name by 'settings'. This
      //could be the complete settings path
      Path=strsub(Path,0,Pos+1)+"settings";
      //If this path was NOT already checked previously, add it to the warning
      //that might be displayed later
      Pos=strstr(Warning,Path);
      if (Pos<0) {
        Warning=Warning+"&nbsp;&nbsp;&nbsp;"+Path+"<br>";
        Count++;
        }
      //If the folder does not exist, try with the next project path
      if (!AWDirEntryExists(Path))
        continue;
      //If the folder exists, return its name
      return Path;
      }
    //If no settings folder could be found and warnings are desired, show
    //a message and give the user the chance to create the settings folder
    //manually. Afterwards, the folder search process is repeated
    if (ShowWarning) {
      if (Count<2)
        Warning=AWLocalise("the following<br>directory (with write access)",
          "das folgende Verzeichnis<br>(mit Schreibrechten) händisch "+
          "erzeugen")+":<br>"+Warning;
      else
        Warning=AWLocalise("one of the<br>following directories (with write "+
          "access)","eins der folgenden<br>Verzeichnisse (mit Schreibrechten)"+
          " händisch erzeugen")+":<br>"+Warning;
      if (ShowWarning==1) {
        Warning=":<html><nobr>"+AWLocalise("This ULP needs to save certain "+
          "settings to work properly.<br>Therefore, you should <b>now</b> "
          "manually create ","Dieses ULP muß bestimmte Einstellungen "+
          "speichern, um korrekt zu<br>funktionieren. Dazu sollten Sie <b>"+
          "jetzt</b> ")+Warning;
        Warning+="<br>"+AWLocalise("Select 'Retry' after successfully "+
          "creating the directory, or<br>'Cancel' to exit the ULP.","Wählen "+
          "Sie \"Wiederholen\", nachdem Sie das Verzeichnis erfolgreich<br>"+
          "erzeugt haben, oder \"Abbrechen\", um das ULP zu beenden.");
        Result=dlgMessageBox(Warning,AWLocalise("Retry","Wiederholen"),
          AWLocalise("Cancel","Abbrechen"));
        }
      else {
        Warning=":<html><nobr>"+AWLocalise("In order for some ULPs to "+
          "properly save their settings,<br>you should <b>now</b> manually "+
          "create ","Damit einige ULPs ihre Einstellungen speichern<br>"+
          "können, sollten Sie <b>jetzt</b> ")+Warning;
        Warning+="<br>"+AWLocalise("Select 'OK' after sucessfully creating "+
          "the directory.","Wählen Sie \"OK\", nachdem Sie das Verzeichnis"+
          "<br>erfolgreich erzeugt haben.");
        Result=dlgMessageBox(Warning);
        }
      }
    else
      //The user cancelled the dialog box
      Result=1;
    }
  //No settings folder found
  return "";
  }

//PROCEDURES FOR INI FILE (OR ATTRIBUTE) ACCESS -------------------------------

string AWIniFile[];        //All string entries from the current INI file
                           //(or INI attribute)
int AWIniFileLineCount=0;  //Number of lines in this INI file

int AWReadIniFile(string FileName) {
  //Reads all lines from the INI file FILENAME and puts them into the
  //corresponding global array. Returns 1 if the INI file could be read and
  //contains at least one line. The resulting strings still contain the escape
  //characters \\ (for backslash), \n (for linefeed), \t (for tab), \b (for
  //bar) and \e (for equal)
  int Lines,Nr;
  //Exit if the INI file does not exist
  AWIniFile[0]="";
  AWIniFileLineCount=0;
  FileName=AWTrim(FileName);
  if ((!FileName) || (!AWDirEntryExists(FileName)))
    return 0;
  //Read the file. Exit, if it contains no lines
  Lines=fileread(AWIniFile,FileName);
  if (Lines<1)
    return 0;
  //By some reason, the EAGLE function 'lookup' doesn't work properly if the
  //INI file contains empty lines. Therefore, replace empty lines by lines
  //containing a space character
  for (Nr=0;Nr<Lines;Nr++)
    if (AWIniFile[Nr]=="")
      AWIniFile[Nr]=" ";
  //The INI file contains some lines
  return 1;
  }

int AWReadIniAttribute(string AttributeName,int Schematic,int Board) {
  //Reads the INI settings from the global attribute ATTRIBUTENAME. The first
  //information in the corresponding attribute value should be an international
  //date plus time of the last change. The attribute is extracted from both
  //schematics and board, the newer one of these is taken as source for all
  //INI settings that are put into the corresponding global array. Returns 1 if
  //at least one setting was found. The resulting strings still contain the
  //escape characters \\ (for backslash), \n (for linefeed), \t (for tab), \b
  //(for bar) and \e (for equal)
  string SettingsSCH="",SettingsBRD="",Settings="";
  int Lines,Nr;
  //Remove the current INI array contents
  AWIniFile[0]="";
  AWIniFileLineCount=0;
  AttributeName=strupr(AWTrim(AttributeName));
  //Read the schematics attribute, if available
  if ((Schematic) && (project.schematic))
    project.schematic(S)
      S.attributes(A)
        if (A.name==AttributeName)
          SettingsSCH=AWTrim(A.value);
  //Read the board attribute, if available
  if ((Board) && (project.board))
    project.board(B)
      B.attributes(A)
        if (A.name==AttributeName)
          SettingsBRD=AWTrim(A.value);
  //Select the newer of both
  if ((SettingsSCH) && (SettingsBRD)) {
    if (SettingsSCH>SettingsBRD)
      Settings=SettingsSCH;
    else
      Settings=SettingsBRD;
    }
  else if (SettingsSCH)
    Settings=SettingsSCH;
  else
    Settings=SettingsBRD;
  //Extract all lines from the attribute value. Exit, if it contains no lines
  if (!Settings)
    return 0;
  Lines=strsplit(AWIniFile,Settings,'|');
  if (Lines<1)
    return 0;
  //By some reason, the EAGLE function 'lookup' doesn't work properly if the
  //INI file contains empty lines. Therefore, replace empty lines with lines
  //containing a space character
  for (Nr=0;Nr<Lines;Nr++)
    if (!AWIniFile[Nr])
      AWIniFile[Nr]=" ";
  //The INI attribute contains some lines
  return 1;
  }

int AWReadIniSettings(string AttributeName,string FileName) {
  //Reads the INI settings from the global attribute ATTRIBUTENAME, if this is
  //available. If not, reads the settings from the INI file FILENAME. The
  //INI lines are put into the corresponding global array. Returns 1 if the INI
  //settings could be read and contain at least one line. The resulting strings
  //still contain the escape characters \\ (for backslash), \n (for linefeed),
  //\t (for tab), \b (for bar) and \e (for equal)
  int OK=0;
  //Read the INI attribute. Exit on success
  if (AttributeName)
    OK=AWReadIniAttribute(AttributeName,1,1);
  if (OK)
    return 1;
  //Otherwise, read the INI file
  if (FileName)
    OK=AWReadIniFile(FileName);
  if (OK)
    return 1;
  return 0;
  }

string AWReadIniString(string Key,string Default) {
  //Reads the KEY string from the current INI file array and returns it with
  //escape sequences properly converted to original characters. If no entry is
  //found, returns DEFAULT
  string Text,Char;
  int Pos;
  Text=lookup(AWIniFile,Key,1,'=');
  if (Text)
    return AWUnescapeString(Text);
  return Default;
  }

string AWWriteIniString(string Key,string Text) {
  //Returns the string necessary to write the TEXT into the KEY entry of an INI
  //file or attribute. Special characters are automatically escaped with a
  //backslash
  Key=AWTrim(Key);
  if (Key)
    return AWEscapeString(Key,1,1)+"="+AWEscapeString(Text,1,1)+"|";
  return AWEscapeString(Text,1,1)+"|";
  }

string AWWriteIniSection(string Section) {
  //Returns the string necessary to write the SECTION string of an INI file or
  //attribute. Special characters are automatically escaped with a backslash,
  //the section is framed by brackets
  Section=AWTrim(Section);
  if (Section)
    Section="["+Section+"]";
  return "|"+AWWriteIniString("",Section);
  }

int AWReadIniInt(string Key,int Minimum,int Maximum,int Default) {
  //Reads the KEY integer entry from the current INI file array. If the value
  //exceeds the range from MINIMUM to MAXIMUM, or if no entry is found, DEFAULT
  //is returned
  string IniString;
  int Result;
  IniString=lookup(AWIniFile,Key,1,'=');
  if (IniString=="")
    return Default;
  Result=strtol(IniString);
  if ((Result<Minimum) || (Result>Maximum))
    Result=Default;
  return Result;
  }

string AWWriteIniInt(string Key,int Value) {
  //Returns the string necessary to write the integer VALUE into the KEY entry
  //of an INI file or attribute
  string Text;
  sprintf(Text,"%d",Value);
  return AWWriteIniString(Key,Text);
  }

real AWReadIniReal(string Key,real Minimum,real Maximum,real Default) {
  //Reads the KEY real number entry from the current INI file array. If the
  //value exceeds the range from MINIMUM to MAXIMUM, or if no entry is found,
  //DEFAULT is returned
  string IniString;
  real Result;
  IniString=lookup(AWIniFile,Key,1,'=');
  if (IniString=="")
    return Default;
  Result=strtod(IniString);
  if ((Result<Minimum) || (Result>Maximum))
    Result=Default;
  return Result;
  }

string AWWriteIniReal(string Key,real Value) {
  //Returns the string necessary to write the real number VALUE into the KEY
  //entry of an INI file or attribute
  string Text;
  sprintf(Text,"%g",Value);
  return AWWriteIniString(Key,Text);
  }

int AWReadIniBool(string Key,int Default) {
  //Reads the KEY boolean entry from the current INI file array. If no
  //entry is found, returns DEFAULT
  return AWReadIniInt(Key,0,1,Default);
  }

void AWWriteIniFile(string FileName,string Info,int Version,string Settings) {
  //Saves the INI settings SETTINGS to the file FILENAME. If VERSION!=0, this
  //file version number is included as a separate line. If INFO='auto', some
  //hint warning the user not to edit this file manually is included. If
  //INFO!="", this information text is included instead. Additionally, the
  //current time stamp is included as a separate line
  FileName=AWTrim(FileName);
  if (!FileName)
    return;
  //Include file version, if desired
  if (Version)
    Settings=AWWriteIniInt("FileVersion",Version)+Settings;
  //Always add a time stamp
  Settings=AWWriteIniString("FileTime",AWNowText(1))+Settings;
  //Include file information, if desired
  Info=AWTrim(Info);
  if (Info) {
    if (strupr(Info)=="AUTO")
      Settings="Used by '"+filename(argv[0])+"' - do not edit manually!|"+
        Settings;
    else
      Settings=AWEscapeString(Info,1,1)+"|"+Settings;
    Settings="[FileInformation]|FileInfo="+Settings;
    }
  //Replace newly inserted bars by line feeds
  Settings=AWReplace(Settings,"|","\n");
  //Write the file
  output(FileName)
    printf("%s",Settings);
  }

string AWWriteIniAttribute(string AttributeName,string Info,int Version,
    string Settings,int Schematic,int Board) {
  //Returns the exit command string necessary to save the INI settings SETTINGS
  //in the global attribute ATTRIBUTENAME. If VERSION!=0, this file version
  //number is included as a separate line. If INFO='auto', some hint warning
  //the user not to edit this file manually is included. If INFO!="", this
  //information text is included instead. Additionally, the current time stamp
  //is included as a separate line. If SCHEMATIC!=0, the global attribute of
  //the schematic is set. If BOARD!=0, the global attribute of the board is
  //set. If neither SCHEMATIC nor BOARD is given, the attribute of the current
  //editor window is set
  string Text="";
  //If no attribute name is given, exit
  AttributeName=strupr(AWTrim(AttributeName));
  if (!AttributeName)
    return "";
  //Include file version, if desired
  if (Version)
    Settings=AWWriteIniInt("FileVersion",Version)+Settings;
  //Always add a time stamp
  Settings=AWWriteIniString("FileTime",AWNowText(1))+Settings;
  //Include file information, if desired
  Info=AWTrim(Info);
  if (Info) {
    if (strupr(Info)=="AUTO")
      Settings="Used by '"+filename(argv[0])+"' - do not edit manually! |"+
        Settings;
    else
      Settings=AWEscapeString(Info,1,1)+" |"+Settings;
    }
  //If schematic or board attributes are to be set explicitly, do this
  if ((Schematic) || (Board)) {
    if ((Schematic) && (project.schematic)) {
      Text+="EDIT .sch;\n"+
        "ATTRIBUTE * "+AttributeName+" '"+AWText2Script(Settings,0,0)+"';\n";
      }
    if ((Board) && (project.board)) {
      Text+="EDIT .brd;\n"+
        "ATTRIBUTE * "+AttributeName+" '"+AWText2Script(Settings,0,0)+"';\n";
      }
    }
  else
    //Otherwise just use the current editor window
    Text+="ATTRIBUTE * "+AttributeName+" '"+AWText2Script(Settings,0,0)+"';\n";
  return Text;
  }

//PROCEDURES FOR OPERATING SYSTEM DETECTION -----------------------------------

int AWIsWindows() {
  //Returns 1, if EAGLE is running under Windows (0 for Linux or Macintosh)
  if ((strsub(argv[0],0,1)=="/") && (strsub(argv[0],0,2)!="//"))
    return 0;
  return 1;
  }

int AWIsLinux() {
  //Returns 1, if EAGLE is running under Linux (0 for Windows or Macintosh).
  //Since Linux and Macintosh cannot properly be distingished from inside ULPs,
  //in case a non-Windows system is found, the current system type is read out
  //of the 'ostype.ini' file in the settings path. If this file is not
  //available, the user is asked whether a Linux or Macintosh machine is used
  string SettingsPath="";
  int SystemType=0;
  if (!((strsub(argv[0],0,1)=="/") && (strsub(argv[0],0,2)!="//")))
    return 0;
  SettingsPath=AWGetULPSettingsPath(1);
  if (!SettingsPath)
    exit(0);
  if (AWReadIniFile(SettingsPath+"/ostype.ini"))
    SystemType=AWReadIniInt("SystemType",0,2,0);
  if (!SystemType) {
    SystemType=dlgMessageBox(":<html><nobr>"+AWLocalise("You are running "+
      "EAGLE on either a <b>Macintosh</b> or a <b>Linux</b> machine.<br><br>"+
      "Since these two cannot be properly distinguished automatically,<br>"+
      "please select your current system type manually.","Sie verwenden EAGLE"+
      " auf einem <b>Macintosh</b> oder unter <b>Linux</b>.<br><br>Da diese "+
      "nicht automatisch unterschieden werden können, geben Sie<br>den "+
      "aktuellen Systemtyp bitte manuell an."),"-&Linux","&Macintosh");
    SystemType++;
    AWWriteIniFile(SettingsPath+"/ostype.ini","Used to distinguish between "+
      "Macintosh and Linux systems - do not edit manually!",0,
      AWWriteIniSection("System")+AWWriteIniInt("SystemType",SystemType));
    }
  if (SystemType==1)
    return 1;
  return 0;
  }

int AWIsMac() {
  //Returns 1, if EAGLE is running on a Macintosh (0 for Windows or Linux).
  //Since Linux and Macintosh cannot properly be distingished from inside ULPs,
  //in case a non-Windows system is found, the current system type is read out
  //of the "ostype.ini" file in the settings path. If this file is not
  //available, the user is asked whether a Linux or Macintosh machine is used
  if (AWIsWindows())
    return 0;
  if (AWIsLinux())
    return 0;
  return 1;
  }



int AWParameterFound(string Name,int Abbreviate) {
  //Returns 1 if the ULP was started with the (case-insensitive) command line
  //parameter NAME (0 otherwise). If ABBREVIATE<>0, the parameter is also found
  //when it was typed abbreviated
  int Nr;
  string Text;
  //Exit if no parameter or no name was given
  Name=strupr(AWTrim(Name));
  if ((argc<2) || (!Name))
    return 0;
  //Check all given parameters separately
  for (Nr=1;Nr<argc;Nr++) {
    Text=strupr(argv[Nr]);
    if (((Abbreviate) && (strsub(Name,0,strlen(Text))==Text)) || (Name==Text))
      return 1;
    }
  //Desired parameter not found
  return 0;
  }

string AWAttributeText(string Text) {
  int Pos;
  //Replace double backslashes by single ones
  Pos=strstr(Text,"\\\\");
  while (Pos!=-1) {
    Text=strsub(Text,0,Pos+1)+strsub(Text,Pos+2);
    Pos=strstr(Text,"\\\\",Pos+1);
    }
  //Replace escaped exclamation mark my normal one
  Pos=strstr(Text,"\\!");
  while (Pos!=-1) {
    Text=strsub(Text,0,Pos)+strsub(Text,Pos+1);
    Pos=strstr(Text,"\\!",Pos+1);
    }
  return Text;
  }

int AWBadCharsList[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,
  22,23,24,25,26,27,28,29,30,31,127,128,129,130,131,132,133,134,135,136,137,
  138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,
  156,157,158,159,160,162,164,165,166,167,168,169,173,174,175,179,180,182,
  183,184,185,190,192,193,194,195,200,202,203,204,205,206,207,208,210,211,
  212,213,215,217,218,219,222,227,240,245,253,254,255};
  //If any of these character values is found in an edit field string, it
  //is probable that EAGLE does NOT correctly interpret it
int AWBadChars[];
  //This array holds 256 entries, each containing 0 for an acceptable character
  //code and 1 for a bad character

void AWCreateBadIndex() {
  //Create the BADCHARS table from the BADCHARSLIST
  int Nr;
  //Set all characters to "acceptable"
  for (Nr=0;Nr<256;Nr++)
    AWBadChars[Nr]=0;
  for (Nr=0;Nr<256;Nr++)
    AWBadChars[AWBadCharsList[Nr]]=1;
  }


string AWRealToStr(real x) {
  //Returns the real number X formatted as string (fixed point, no exponent)
  string Text;
  int DotPos,Nr;
  sprintf(Text,"%f",x);
  DotPos=strstr(Text,".");
  if (DotPos>=0)
    for (Nr=strlen(Text)-1;Nr>=DotPos;Nr--) {
      if ((Text[Nr]!='0') && (Text[Nr]!='.'))
        break;
      else
        Text=strsub(Text,0,strlen(Text)-1);
      }
  return Text;
  }

string AWIntToStr(int x) {
  //Returns the integer X formatted as string
  string Text;
  sprintf(Text,"%d",x);
  return Text;
  }




// Procedures for file access and file name manipulation ----------------------

string AWProjectName(void) {
  //Returns one of the filenames of the current project. If no project is
  //available, an empty string is returned
  if (project.board)
    project.board(B)
      return B.name;
  if (project.schematic)
    project.schematic(S)
      return S.name;
  return "";
  }

string AWProjectFolder(void) {
  //Returns the name of the current project's folder (including trailing
  //slash). If no project is available, an empty string is returned
  string Result;
  Result=AWProjectName();
  if (Result)
    Result=filedir(Result);
  return Result;
  }

string AWSafeFileName(string Text,string SubFolder,string Message) {
  //Converts spaces IN THE FILENAME PART of the string TEXT (and NOT in the
  //path, because the path of the current project cannot be renamed) to
  //underscores. Additionally, if SUBFOLDER is given and the subfolder exists
  //on the drive, this is inserted between the pathname and the filename. If
  //the SUBFOLDER does NOT exist and a MESSAGE is defined, this message is
  //shown to enable the user to create the folder manually. Converting the
  //spaces is necessary, because even if some TeX versions can cope with spaces
  //in filenames, most DVI previewers cannot. Since one cannot force people NOT
  //to use the bloody spaces in their filenames, these must be corrected before
  //processing
  string Directory,FileName;
  int Pos;
  //Extract pathname and filename from the string and convert spaces
  Directory=filedir(Text);
  FileName=AWReplace(filename(Text)," ","_");
  //Take the filename without extension and replace all dots by underscores
  //(PDFLaTeX cannot cope with dots in filenames)
  FileName=AWReplace(filesetext(FileName,""),".","_")+fileext(FileName);
  //Add the subfolder name, if available
  if (SubFolder!="") {
    if ((!AWDirEntryExists(Directory+SubFolder)) && (Message)) {
      if (strstr(Message,"%s")>=0)
        sprintf(Message,Message,Directory+SubFolder);
      dlgMessageBox(Message);
      }
    if (AWDirEntryExists(Directory+SubFolder))
      Directory+=SubFolder+"/";
    }
  //Return the new complete filename
  return Directory+FileName;
  }

string AWGetIniReadFileName() {
  //Returns the complete file name of the ULP's initialisation file in order to
  //read from it. If a file with the ULP's name and extension "ini" is found
  //in the settings path, this file is used. If not, a corresponding file is
  //searched in the ULP's folder. If it is not found there, either, an empty
  //string is returned
  string FileName;
  //Look in the settings folder for the INI file
  FileName=AWGetULPSettingsPath(0);
  if (FileName!="") {
    FileName+="/"+filesetext(filename(argv[0]),".ini");
    if (!AWDirEntryExists(FileName))
      FileName="";
    }
  //If no file could be found, search in the ULP's folder
  if (FileName=="") {
    FileName=filesetext(argv[0],".ini");
    if (!AWDirEntryExists(FileName))
      FileName="";
    }
  //Return either a correct file name or an empty string
  return FileName;
  }

string AWGetIniWriteFileName() {
  //Returns the complete file name of the ULP's initialisation file in order to
  //write to it. Because the ULP's directory might well be write-protected,
  //the INI file may only reside in the special settings folder (which MUST
  //be write-enabled). If the settings folder was found, it is returned
  //including the ULP's name and the extension "ini". If not, the user is asked
  //to create the settings folder in order for the ULP to work properly. If
  //no settings folder is created by the user, an empty string is returned
  string FileName;
  FileName=AWGetULPSettingsPath(1);
  if (FileName!="")
    FileName+="/"+filesetext(filename(argv[0]),".ini");
  return FileName;
  }

int AWExecuteProgram(string FileName,string Parameters,int Wait) {
  string CommandLine="";
  //Exit, if the file doesn't exist
  if ((!FileName) || (!AWDirEntryExists(FileName)))
    return -1;
  if (AWIsWindows()) {
    if (!Wait)
      //Under Windows, EAGLE can NOT decently start a program and immediately
      //return to the ULP: The help file mentions the use of 'cmd.exe', but
      //this 1. opens an undesired DOS shell and 2. doesn't work properly,
      //anyway. Therefore, run the compiled Delphi executable
      //'startprogram.exe', which itself starts the desired program and
      //immediately returns. In this case, the desired program is put into
      //a command line argument, which makes it necessary to convert slashes
      //to backslashes in order to run everything properly
      CommandLine="\""+filedir(argv[0])+"startprogram.exe\" \""+
        AWReplace(FileName,"/","\\")+"\"";
    else
      //If EAGLE should wait for the program to finish, it can just be called
      //directly
      CommandLine="\""+FileName+"\"";
    //If parameters are available, put them into the command line
    if (Parameters)
      CommandLine+=" \""+Parameters+"\"";
    }
  else if (AWIsLinux()) {
    //Under Linux, just put filename and parameters into the command line and
    //add a '&', if immediate return is desired
    CommandLine="'"+FileName+"'";
    if (Parameters)
      CommandLine+=" '"+Parameters+"'";
    if (!Wait)
      CommandLine+=" &";
    }
  else {
    //On a Macintosh, programs are started with the 'open' command
    CommandLine="open -a '"+FileName+"'";
    if (Parameters)
      CommandLine+=" '"+Parameters+"'";
    if (!Wait)
      CommandLine+=" &";
    }
  return system(CommandLine);
  }

int AWCreateFolder(string Name) {
  //Creates the folder NAME including all necessary parent folders and returns
  //1 on success (0 otherwise)
  int Result;
  if (strsub(Name,strlen(Name)-1)=="/")
    Name=strsub(Name,0,strlen(Name)-1);
  if (AWIsWindows())
    Result=system("\""+AWReplace(filedir(argv[0])+"filetools.exe\" "+
      "createdir \""+Name+"\"","/","\\"));
  else
    Result=system("mkdir -p \""+Name+"\"");
  return (!Result) & AWDirEntryExists(Name);
  }

int AWFolderExists(string Name) {
  //Returns 1 if the folder NAME exists (0 otherwise)
  if (!AWDirEntryExists(Name))
    return 0;
  //If the directory entry NAME exists, it still might not be a directory, but
  //a file (EAGLE makes no distinction between those). Therefore, in case the
  //entry exists, try to create a folder of the same name. If this works, the
  //folder already exists. If this fails, the file system was not changed, but
  //the directory entry is a file and NOT a folder
  return AWCreateFolder(Name);
  }

int AWFileExists(string Name) {
  //Returns 1 if the file (and NOT the folder) NAME exists (0 otherwise)
  if (!AWDirEntryExists(Name))
    return 0;
  //If the directory entry NAME exists, it still might not be a file, but a
  //folder (EAGLE makes no distinction between those). Therefore, in case the
  //entry exists, try to create a folder of the same name. If this fails, the
  //file already exists. If this works, the file system was not changed, but
  //the directory entry is a folder and NOT a file
  return !AWCreateFolder(Name);
  }

int AWDeleteFile(string Name) {
  //Deletes the file NAME and returns 1 on success (0 otherwise)
  int Result;
  if (strsub(Name,strlen(Name)-1)=="/")
    Name=strsub(Name,0,strlen(Name)-1);
  if (!AWFileExists(Name))
    return 1;
  if (AWIsWindows())
    Result=system("\""+AWReplace(filedir(argv[0])+"filetools.exe\" "+
      "deletefile \""+Name+"\"","/","\\"));
  else
    Result=system("rm -f \""+Name+"\"");
  return (!Result) & (!AWFileExists(Name));
  }

int AWDeleteFolder(string Name) {
  //Deletes the folder NAME including ALL its files and subdirectories and
  //returns 1 on success (0 otherwise)
  int Result;
  if (strsub(Name,strlen(Name)-1)=="/")
    Name=strsub(Name,0,strlen(Name)-1);
  if (!AWFolderExists(Name))
    return 1;
  if (AWIsWindows())
    Result=system("\""+AWReplace(filedir(argv[0])+"filetools.exe\" "+
      "deletedir \""+Name+"\"","/","\\"));
  else
    Result=system("rm -d -f -r \""+Name+"\"");
  return (!Result) & (!AWFolderExists(Name));
  }

int AWCopyTextFile(string SourceName,string DestinationName) {
  //Copies the file SOURCENAME to a new file with the name DESTINATIONNAME and
  //returns 1 on success (0 otherwise)
  int Result,Size,Lines;
  string Contents;
  if (!AWFileExists(SourceName))
    return 0;
  Lines=fileread(Contents,SourceName);
  if (Lines<1)
    return 0;
  if (!AWCreateFolder(filedir(DestinationName)))
    return 0;
  output(DestinationName)
    printf("%s",Contents);
  return 1;
  }

int AWCreateExportFolder() {
  //Creates the subfolder 'doc/export' in the project folder and creates its
  //DESCRIPTION file. Returns 1 on success (0 otherwise)
  string FileName;
  FileName=AWProjectFolder()+"doc/export/";
  if (!AWCreateFolder(FileName))
    return 0;
  output(FileName+"DESCRIPTION")
    printf("%s","<table width=\"100%\" cellpadding=\"5\" cellspacing=\"0\" "+
        "bgcolor=\"#E0E0E0\">\n"+
      "<tr><td><h3>Exported files</h3>\n"+
      "...and other user-defined support files</td></tr></table><br><br>\n"+
      "This folder contains bitmaps and PDF files exported by the ULPs "+
        "<b>bitmap</b>\n"+
      "or <b>pdfprint</b>.<p><hr>\n"+
      "<b>Hints:</b>\n"+
      "<ol><li>You can also place additional files here, but should avoid "+
        "naming them\n"+
      "with the postfixes <b>-board</b>, <b>-schematic</b> or <b>-sheet*"+
        "</b>, since\n"+
      "these could be automatically overwritten.</li></ol>");
  return 1;
  }

//----- LAYER PROCEDURES -----

int AWIsBRDLayerUsed(UL_BOARD B,int Layer) {
  //Returns 1, if something is drawn in the LAYER (but pads or vias) of the
  //board B (0 otherwise)
  B.circles(C)
    if (C.layer==Layer)
      return 1;
  B.elements(E) {
    E.texts(T)
      if (T.layer==Layer)
        return 1;
    E.package.circles(C)
      if (C.layer==Layer)
        return 1;
    E.package.polygons(P)
      if (P.layer==Layer)
        return 1;
    E.package.rectangles(R)
      if (R.layer==Layer)
        return 1;
    E.package.texts(T)
      if (T.layer==Layer)
        return 1;
    E.package.wires(W)
      if (W.layer==Layer)
        return 1;
    }
  B.polygons(P)
    if (P.layer==Layer)
      return 1;
  B.rectangles(R)
    if (R.layer==Layer)
      return 1;
  B.signals(S)
    S.wires(W)
      if (W.layer==Layer)
        return 1;
  B.texts(T)
    if (T.layer==Layer)
      return 1;
  B.wires(W)
    if (W.layer==Layer)
      return 1;
  return 0;
  }

//Graphical procedures

real AWTextWidth(string Text,int Start,int Stop,real TextHeight) {
  //Calculates the graphical text width of a substring of TEXT, beginning with
  //START and ending with STOP, using a vector font with the TEXTHEIGHT
  //Global length of all ASCII characters (from 0 to 255), each given in mm
  //for 10 characters of 8mm size (necessary for text width calculation)
  real CharWidths[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,74,25,61.5,74,74,86.5,74,42,49.5,49.5,74,74,49.5,74,37,74,74,74,74,
    74,74,74,74,74,74,74,37.5,49.5,74,74,74,74,74,74,74,74,74,74,74,74,74,49.5,
    74,74,74,74,74,74,74,74,74,74,74,74,74,74,74,74,74,49.5,74,49.5,74,74,37,
    74,74,74,74,74,49.5,74,74,49.5,49.5,61.5,49.5,74,74,74,74,74,61.5,74,49.5,
    74,74,74,74,74,74,61.5,25,61.5,74,74,110.5,98.5,110.5,12.5,49.5,49.5,24.5,
    49.5,49.5,24.5,74,49.5,49.5,24.5,24.5,74,49.5,49.5,49.5,74,74,74,49.5,74,
    74,49.5,74,74,74,49.5,49.5,98.5,74,24.5,74,86,49.5,49.5,25.5,61.5,37,49.5,
    74,74,74,74,74,61.5,61.5,74,49.5,74,74,74,61.5,37,61.5,86,74,74,86,86,74,
    74,74,49.5,74,49.5,74,74,123,74,24.5,74,49.5,49.5,49.5,49.5,74,49.5,49.5,
    74,86,86,61.5,86,74,98.5,98.5,61.5,74,74,74,74,74,74,74,74,74,24.5,74,74,
    110.5,61.5,74,74,74,74,61.5,49.5,61.5,49.5,49.5,74,74,74,74,74,74,61.5,74,
    74,74,74,74,98.5,86,74};
  int Nr;
  real Width;
  //Correct input parameters, if necessary
  if (Start>Stop) {
    Nr=Start;
    Start=Stop;
    Stop=Nr;
    }
  if (Start<0)
    Start=0;
  if (Start>=strlen(Text))
    return 0;
  if (Stop>=strlen(Text))
    Stop=strlen(Text)-1;
  //Sum up the measured width of all individual string characters. Since
  //these were measured in mm for 10 characters at 8mm heigth, divide by 80
  //and multiply by the desired text height
  Width=0;
  for (Nr=Start;Nr<=Stop;Nr++)
    Width+=CharWidths[Text[Nr]];
  return Width/80*TextHeight;
  }

int AWGetFirstStringLength(string Text,real Height,real MaxWidth) {
  //Checks, whether the string TEXT with the text HEIGHT fits into the
  //available MAXWIDTH and returns the length of the first text line. If this
  //is the length of the original text, the whole string fits into the first
  //line. If this is smaller than the text length, it represents the length
  //of the first line, breaking the text at a space or behind a hyphen. If
  //the result is 0, the text does NOT fit in two lines. If -1 is returned,
  //no text was given
  int Pos;
  //If no text is given, exit with -1
  if (Text=="")
    return 0;
  //If all text fits the first line, exit with text length
  if (AWTextWidth(Text,0,strlen(Text)-1,Height)<=MaxWidth)
    return strlen(Text);
  //2 lines are necessary. Search the break point
  Pos=strlen(Text)-2;
  while (Pos>0) {
    //If the text rest doesn't fit the second line, exit with 0
    //if (TextWidth(Text,Pos+1,strlen(Text)-1,Height)>MaxWidth)
    //  return 0;
    //For hyphens, the break point is AT the hyphen
    if ((Text[Pos]=='-') && (AWTextWidth(Text,0,Pos,Height)<=MaxWidth))
      break;
    //For spaces, the break point is BEFORE the space
    if ((Text[Pos]==' ') && (AWTextWidth(Text,0,Pos-1,Height)<=MaxWidth)) {
      Pos--;
      break;
      }
    //Neither hyphen nor space => Shorten first line
    Pos--;
  }
  //No break found? Then exit with 0
  if (Pos<1)
    return -strlen(Text);
  //Otherwise, return the length of the first line
  if (AWTextWidth(AWTrim(strsub(Text,Pos+1)),0,1000,Height)>MaxWidth)
    return -Pos-1;
  else
    return Pos+1;
  }

enum {cefTooShort,cefTooLong,cefIllegalChar,cefTooWide,cefOK};

int AWCheckEditField(string Text,string FieldEnglish,string FieldGerman,
    int MinLength,int MaxLength,real Height,real MaxWidth,int TwoLines) {
  //Checks the string TEXT for problems: Warnings or error messages are
  //displayed if 1) the text length is less than MINLENGTH, 2) MAXLENGTH>0 and
  //the text length is larger than MAXLENGTH, 3) the text contains unsupported
  //characters or 4) the real width of the text, when written with the given
  //text HEIGHT and distributed over TWOLINES (if desired), exceeds the
  //MAXWIDTH. Depending on the problem, the user is asked whether the text
  //should be revised. FIELDENGLISH and FIELDGERMAN are the descriptive names
  //of the edit field containing the TEXT. Returns 0 if the text needs revision
  //and 1 if the text can be used as it is
  int Nr;
  string Message,Start,NrText;
  Text=AWTrim(Text);
  //Create the beginning of the message including the field name
  if ((FieldEnglish) && (FieldGerman))
    Start=AWLocalise("<html>The field <b>"+FieldEnglish+"</b> ",
      "<html>Das Feld <b>"+FieldGerman+"</b> ");
  else
    Start=AWLocalise("<html>The edit field ","<html>Das Eingabefeld ");
  //Exit with error if the text is not long enough
  if (strlen(Text)<MinLength) {
    Message=":"+Start+AWLocalise("does not contain enough text<br>"+
      "(at least ","enthält zuwenig Text<br>(mindestens ");
    sprintf(NrText,"%u",MinLength);
    Message+=NrText+AWLocalise(" character"," Zeichen");
    if (MinLength>1)
      Message+=AWLocalise("s","");
    Message+=AWLocalise(" necessary"," erforderlich")+").";
    dlgMessageBox(Message);
    return cefTooShort;
    }
  //If the text is too long, show a warning. If the user decides to revise the
  //text, exit with error
  if ((MaxLength>0) && (strlen(Text)>MaxLength)) {
    Message="!"+Start+AWLocalise("contains too much text<br>(maximum ",
      "enthält zuviel Text<br>(maximal ");
    sprintf(NrText,"%u",MaxLength);
    Message+=NrText+AWLocalise(" character"," Zeichen");
    if (MaxLength>1)
      Message+=AWLocalise("s","");
    Message+=AWLocalise(" allowed).<br>If you proceed now, excess text will be "+
      "deleted.<br><br>Do you want to revise the text?"," erlaubt).<br>Wenn "+
      "Sie jetzt fortfahren, wird überzähliger Text gelöscht.<br><br>"+
      "Möchten Sie den Text revidieren?");
    if (!dlgMessageBox(Message,AWLocalise("-Revise","-Revidieren"),
        AWLocalise("+Proceed","+Fortfahren")))
      return cefTooLong;
    }
  //Here, everything's OK if the string is empty
  if (strlen(Text)==0)
    return cefOK;
  //Check all characters whether they are unsupported or not
  for (Nr=0;Nr<strlen(Text);Nr++)
    if ((Text[Nr]>255) || (AWBadChars[Text[Nr]])) {
      //Unsupported? Then build the warning message:
      Message="!"+Start+AWLocalise("contains unsupported characters<br>that "+
        "cannot be properly displayed","enthält nicht-unterstützte Zeichen,"+
        "<br>die nicht korrekt angezeigt werden können")+":<br>&nbsp;&nbsp;"+
        "&nbsp;&nbsp;"+AWLocalise("'","\"");
      //Insert the first 32 characters from text in HTML syntax (pay attention
      //with '<' and '>' signs) with unsupported characters displayed bold
      Nr=0;
      while ((Nr<32) && (Nr<strlen(Text))) {
        if (Text[Nr]=='<')
          Message+="&lt;";
        else if (Text[Nr]=='>')
          Message+="&gt;";
        else if ((Text[Nr]>255) || (AWBadChars[Text[Nr]]))
          Message+="<b>"+strsub(Text,Nr,1)+"</b>";
        else
          Message+=strsub(Text,Nr,1);
        Nr++;
        }
      Message+=AWLocalise("'","\"");
      if (strlen(Text)>32)
        Message+="...";
      //Display the warning. If the user decides to revise the text, exit
      //with error
      Message+="<br>"+AWLocalise("If you proceed now, EAGLE will replace these "+
        "characters<br>by question marks or other symbols.<br><br>Do you "+
        "want to revise the text?","Wenn Sie jetzt fortfahren, ersetzt EAGLE "+
        "diese Buchstaben<br>durch Fragezeichen oder andere Symbole.<br><br>"+
        "Möchten Sie den Text revidieren?");
      if (dlgMessageBox(Message,AWLocalise("-Revise","-Revidieren"),
          AWLocalise("+Proceed","+Fortfahren"))==0)
        return cefIllegalChar;
      break;
      }
  Nr=0;
  if (MaxWidth>0) {
    if (TwoLines)
      Nr=AWGetFirstStringLength(Text,Height,MaxWidth);
    else if (AWTextWidth(Text,0,strlen(Text),Height)>MaxWidth)
      Nr=-1;
    }
  if (Nr<0) {
    Message="!"+Start+AWLocalise("contains text that does<br>not completely "+
      "fit into the available space.<br>If you proceed now, the text will "+
      "flow out of its frame.<br><br>Do you want to revise the text?",
      "enthält Text, der nicht<br>vollständig in den verfügbaren Platz paßt."+
      "<br>Wenn Sie jetzt fortfahren, fließt der Text<br>aus seinem Rahmen "+
      "heraus.<br><br>Möchten Sie den Text revidieren?");
    if (!dlgMessageBox(Message,AWLocalise("-Revise","-Revidieren"),
        AWLocalise("+Proceed","+Fortfahren")))
      return cefTooWide;
    }
  //The text is OK
  return cefOK;
  }

//Procedures for manipulation of global attributes

string AWReadAttribute(string Name,int SchematicFirst) {
  //Reads the value of the global attribute NAME from both schematics and
  //board.
  string schValue="",brdValue="";
  //Return an empty string in libraries
  if (library)
    return "";
  //Attribute names are always uppercase
  Name=strupr(AWTrim(Name));
  //Read the attribute from the schematics
  if (project.schematic)
    project.schematic(S)
      S.attributes(A)
        if (A.name==Name) {
          schValue=AWAttributeText(A.value);
          break;
          }
  //Read the attribute from the board
  if (project.board)
    project.board(B)
      B.attributes(A)
        if (A.name==Name) {
          brdValue=AWAttributeText(A.value);
          break;
          }
  //
  if (SchematicFirst) {
    if (schValue)
      return schValue;
    else
      return brdValue;
    }
  else if (brdValue)
    return brdValue;
  else
    return schValue;
  }

string AWWriteAttribute(string Name,string Value,int DeleteOnEmpty) {
  //Returns the script string necessary for setting the global attribute NAME
  //to the string VALUE. If DELETEONEMPTY<>0, the attribute is deleted if its
  //value is an empty string
  if (AWTrim(Name)=="")
    return "";
  else if ((Value!="") || (!DeleteOnEmpty))
    return "ATTRIBUTE * "+strupr(AWTrim(Name))+" '"+AWText2Script(Value,1,1)+"';\n";
  else
    return "ATTRIBUTE * "+strupr(AWTrim(Name))+" DELETE;\n";
  }

//

void AWLabel(string EnglishText,string GermanText) {
  dlgLabel("<html><nobr>"+AWLocalise(EnglishText,GermanText)+"</nobr></html>");
  }

void AWMessage(string EnglishText,string GermanText) {
  dlgMessageBox("<html><nobr>"+AWLocalise(EnglishText,GermanText)+
    "</nobr></html>");
  }

void AWWarning(string EnglishText,string GermanText) {
  dlgMessageBox("!<html><nobr>"+AWLocalise(EnglishText,GermanText)+
    "</nobr></html>");
  }

void AWInfo(string EnglishText,string GermanText) {
  dlgMessageBox(";<html><nobr>"+AWLocalise(EnglishText,GermanText)+
    "</nobr></html>");
  }

void AWError(string EnglishText,string GermanText) {
  dlgMessageBox(":<html><nobr>"+AWLocalise(EnglishText,GermanText)+
    "</nobr></html>");
  }

int AWYesNo(string EnglishText,string GermanText) {
  if (dlgMessageBox(";<html><nobr>"+AWLocalise(EnglishText,GermanText)+
      "</nobr></html>",AWLocalise("&Yes","&Ja"),AWLocalise("&No","&Nein"))==0)
    return 1;
  else
    return 0;
  }

int AWContinueCancel(string EnglishText,string GermanText) {
  if (dlgMessageBox(";<html><nobr>"+AWLocalise(EnglishText,GermanText)+
      "</nobr></html>",AWLocalise("&Continue","&Fortfahren"),
      AWLocalise("&Cancel","&Abbrechen"))==0)
    return 1;
  else
    return 0;
  }

int AWCancelContinue(string EnglishText,string GermanText) {
  if (dlgMessageBox(";<html><nobr>"+AWLocalise(EnglishText,GermanText)+
      "</nobr></html>",AWLocalise("-&Cancel","-&Abbrechen"),
      AWLocalise("&Continue","&Fortfahren"))==0)
    return 0;
  else
    return 1;
  }

void AWExitOnLibrary() {
  //Exit, if the program was run from within a library
  if (!library)
    return;
  AWError("This program can only be run from within<br>the <b>schematics</b> "+
    "or <b>board editor</b>!","Dieses Programm kann nur im <b>Schaltplan-"+
    "</b> oder<br><b>Platinen-Editor</b> ausgeführt werden!");
  exit(0);
  }

void AWExitOnNoSchematic() {
  //Exit, if the program was not run from within a schematics
  if (schematic)
    return;
  AWError("This program can only be run from<br>within the <b>schematics "+
    " editor</b>!","Dieses Programm kann nur im<br><b>Schaltplan-Editor</b> "+
    "ausgeführt werden!");
  exit(0);
  }

string AWMic(int Value) {
  //Converts VALUE (given in editor units) to microns and returns the
  //corresponding string
  string Result;
  if (frac(u2mic(Value))==0)
    sprintf(Result,"%d",int(u2mic(Value)));
  else
    sprintf(Result,"%.1f",u2mic(Value));
  return Result;
  }

string AWExtractValueText(string Text) {
  //Extracts the value text from TEXT, being the first (floating point) number
  //in the string, possibly followed by a multiplier prefix. Supported values
  //include, e.g., '4.7n', '33k2', '1e-6', '-1,7e+3M'
  int Pos,Length;
  Text=AWTrim(Text);
  Pos=strxstr(Text,"(^[-+]?[0-9]*[.,]?[0-9]+([eE][-+]?[0-9]+)?"+
    "[fFpPnNuUµmkKMgGtT]?)|(^[-+]?[0-9]+[fFpPnNuUµmkKMgGtTvVrR][0-9]*)",0,
    Length);
  if (Pos<0)
    return "";
  return strsub(Text,Pos,Length);
  }

real AWExtractValue(string Text) {
  //Extracts the value from TEXT, being the first (floating point) number in
  //the string, possibly followed by a multiplier postfix. Supported values
  //include, e.g., '4.7n', '33k2', '1e-6', '-1,7e+3M', '1meg', '4V7'. If no
  //decent value could be extracted, REAL_MAX is returned
  int PostfixNumber=21;
  char PostfixCharacters[]={'f','F','p','P','n','N','u','U','µ','m','k','K',
    'M','g','G','t','T','v','V','r','R'};
  real PostfixMultipliers[]={1e-15,1e-15,1e-12,1e-12,1e-9,1e-9,1e-6,1e-6,1e-6,
    1e-3,1e3,1e3,1e6,1e9,1e9,1e12,1e12,1,1,1,1};
  string Value;
  int Nr,Position;
  real Multiplier=1;
  //Extract the value sub string. Return with REAL_MAX if none was found
  Value=AWExtractValueText(Text);
  if (Value=="")
    return REAL_MAX;
  //Adjust for weird SPICE syntax and check for 'meg': If found, change the
  //lowercase 'm' to uppercase, so that it's properly seen as 1e6
  Position=strstr(Text,Value);
  if ((strsub(Value,strlen(Value)-1)=="m") && (Position>=0) &&
      (strupr(strsub(Text,Position+strlen(Value)-1,3))=="MEG"))
    Value[strlen(Value)-1]='M';
  //Replace commas by dots
  Value=AWReplace(Value,",",".");
  //Search for multiplier postfixes. If found, replace them by a '.' and
  //memorise their multiplier
  for (Nr=0;Nr<PostfixNumber;Nr++) {
    Position=strchr(Value,PostfixCharacters[Nr]);
    if (Position>=0) {
      if (Position==strlen(Value)-1)
        Value=strsub(Value,0,strlen(Value)-1);
      else
        Value[Position]='.';
      Multiplier=PostfixMultipliers[Nr];
      break;
      }
    }
  //In order to avoid binary conversion errors like "0.1u<>100n", round the
  //found value to 12 digits and return this
  sprintf(Value,"%.12e",strtod(Value)*Multiplier);
  return strtod(Value);
  }

string AWExtractToleranceText(string Text) {
  //Extracts the tolerance text from TEXT, being the first (floating point)
  //number followed by a percent character
  int Pos,Length;
  Text=AWTrim(Text);
  Pos=strxstr(Text,"[0-9]*[.,]?[0-9]+[ ]*%",0,Length);
  if (Pos<0)
    return "";
  return strsub(Text,Pos,Length);
  }

real AWExtractTolerance(string Text) {
  //Extracts the tolerance value from TEXT, being the first (floating point)
  //number followed by a percent character. The value is returned in percent.
  //If no decent value could be extracted, REAL_MAX is returned
  return AWExtractValue(AWExtractToleranceText(Text));
  }

string AWExtractTKText(string Text) {
  //Extracts the temperature coefficient text from TEXT, starting with 'TK'
  //and followed by the coefficient value in ppm. Supported values include,
  //e.g., 'TK25', 'TK+-60', 'TK+100-60'
  int Pos,Length;
  Text=AWTrim(Text);
  Pos=strxstr(Text,"[tT][kK][+]?[0-9]*[-]?[0-9]+",0,Length);
  if (Pos<0)
    return "";
  return strsub(Text,Pos,Length);
  }

string AWFormatReal(real Value,int Postfixes) {
  //POSTFIX: 0=no postfixes, 1=postfixes INSIDE numbers (like '4M7'),
  //2=postfixes OUTSIDE numbers (like '4.7M'), 3=SPICE-like postfixes ('4.7Meg')
  int PostfixNr=10;
  string PostfixCharacters[]={"T","G","M","k",".","m","u","n","p","f"};
  real PostfixMultipliers[]={1e12,1e9,1e6,1e3,1,1e-3,1e-6,1e-9,1e-12,1e-15};
  int Pos,Pose,Nr;
  string Postfix=".";
  string Result;
  string Character;
  //Extract the real value from the text. If none could be found, just return
  //the original text
  if (Value>=REAL_MAX)
    return "";
  //If postfixes are desired, reduce the number accordingly
  if (Postfixes)
    for (Nr=0;Nr<PostfixNr;Nr++)
      if (Value>=PostfixMultipliers[Nr]) {
        Value/=PostfixMultipliers[Nr];
        Postfix=PostfixCharacters[Nr];
        if ((Postfixes==3) && (Postfix=="M"))
          Postfix="Meg";
        break;
        }
  //Format the number 'normally'
  if (!Postfixes) {
    if ((Value==0.0) || ((abs(Value)<10000) && (abs(Value)>=0.001)))
      sprintf(Result,"%1.5f",Value);
    else
      sprintf(Result,"%1.3e",Value);
    }
  else if (abs(Value)>=100.0)
    sprintf(Result,"%1.2f",Value);
  else if (abs(Value)>=10.0)
    sprintf(Result,"%1.3f",Value);
  else
    sprintf(Result,"%1.4f",Value);
  //Delete trailing zeros after the decimal dot
  Pos=strchr(Result,'.');
  if (Pos>=0) {
    Pose=strchr(Result,'e');
    if (Pose<0)
      Pose=strlen(Result);
    for (Nr=Pose-1;Nr>=Pos;Nr--) {
      if ((Result[Nr]!='0') && (Result[Nr]!='.'))
        break;
      Result=strsub(Result,0,Pose-1)+strsub(Result,Pose);
      Pose--;
      }
    }
  //Replace the decimal dot by the postfix, if desired
  Pos=strchr(Result,'.');
  if ((Pos>=0) && (Postfixes==1))
    Result=strsub(Result,0,Pos)+Postfix+strsub(Result,Pos+1);
  else if ((Postfixes>0) && (Postfix!="."))
    Result=Result+Postfix;
  return Result;
  }

string AWCreateValueText(string Text,int Postfixes) {
  //POSTFIX: 0=no postfixes, 1=postfixes INSIDE numbers (like '4M7'),
  //2=postfixes OUTSIDE numbers (like '4.7M'), 3=SPICE-like postfixes ('4.7Meg')
  real Value;
  //Extract the real value from the text. If none could be found, just return
  //the original text
  Value=AWExtractValue(Text);
  if (Value>=REAL_MAX)
    return Text;
  return AWFormatReal(Value,Postfixes);
  }

string varNotPopulated="[unpopulated]";

string AWCatchUnpopulatedText(string Text) {
  Text=AWTrim(Text);
  if ((strupr(Text)=="NC") || (strupr(Text)=="NP") || (strupr(Text)=="NOMOUNT")
      || (strupr(Text)=="-") || (strupr(Text)=="--") || (strupr(Text)=="---")
      || (strupr(Text)=="[NOT POPULATED]") || (strupr(Text)=="NOT POPULATED")
      || (strupr(Text)=="[UNPOPULATED]") || (strupr(Text)=="UNPOPULATED") ||
      (strupr(Text)==strupr(varNotPopulated)))
    return varNotPopulated;
  return Text;
  }

int AWMinMaxInt(int Value,int Minimum,int Maximum) {
  int Dummy;
  if (Minimum>Maximum) {
    Dummy=Minimum;
    Minimum=Maximum;
    Maximum=Dummy;
    }
  return min(max(Value,Minimum),Maximum);
  }

int AWMinMaxReal(real Value,real Minimum,real Maximum) {
  real Dummy;
  if (Minimum>Maximum) {
    Dummy=Minimum;
    Minimum=Maximum;
    Maximum=Dummy;
    }
  return min(max(Value,Minimum),Maximum);
  }

enum {awbtUnknown,awbtCadSoft,awbtVersion4,awbtVersion5,awbtMostly4,
  awbtMostly5,awbtMostlyCadSoft};

int AWBoardType() {
  int HasName4,HasName5,Count4=0,Count5=0,CountCadSoft=0,NameFound;
  if (!project.board)
    return awbtUnknown;
  project.board(B)
    B.elements(E) {
      HasName4=0;
      HasName5=0;
      NameFound=0;
      E.texts(T)
        if (T.value==E.name) {
          NameFound=1;
          if ((T.layer==LAYER_TPLACE) || (T.layer==LAYER_BPLACE))
            HasName5=1;
          else if ((T.layer==LAYER_TDOCU) || (T.layer==LAYER_BDOCU))
            HasName4=1;
          }
      E.package.texts(T)
        if (T.value==E.name) {
          NameFound=1;
          if ((T.layer==LAYER_TPLACE) || (T.layer==LAYER_BPLACE))
            HasName5=1;
          else if ((T.layer==LAYER_TDOCU) || (T.layer==LAYER_BDOCU))
            HasName4=1;
          }
      if (NameFound) {
        if (HasName5)
          Count5++;
        if (HasName4)
          Count4++;
        if ((!HasName4) && (!HasName5))
          CountCadSoft++;
        }
      }
  if ((Count5) && (!Count4) && (!CountCadSoft))
    return awbtVersion5;
  if ((Count4) && (!Count5) && (!CountCadSoft))
    return awbtVersion4;
  if ((CountCadSoft) && (!Count4) && (!Count5))
    return awbtCadSoft;
  if ((Count5>=Count4) && (Count5>=CountCadSoft))
    return awbtMostly5;
  if ((Count4>=CountCadSoft) && (Count4>=Count5))
    return awbtMostly4;
  return awbtMostlyCadSoft;
  }

string AWGetPartValue(UL_PART P) {
  //Returns the DISPLAYED value of the part P. This MIGHT be the REAL value of
  //the part, but under the following circumstances, it's not: If the part's
  //value may NOT be changed, and it equals its standard (automatically
  //generated) value, return the value string from the element's 'TYPE'
  //attribute (if available)
  if ((P.device.value=="Off") && (P.value==P.device.name) &&
      (P.attribute["TYPE"]))
    return P.attribute["TYPE"];
  else
    return P.value;
  }

string AWRestoreEditorWindow() {
  //Returns the string necessary to restore the previous editor window,
  //independent of the current one
  string Result;
  if (board)
    Result="EDIT .brd;\n";
  if (schematic) {
    Result="EDIT .sch;\n";
    if (sheet)
      sheet(S)
        Result+="EDIT .s"+AWIntToStr(S.number)+";\n";
    }
  return Result;
  }

void AWInsertOKCancel() {
  dlgHBoxLayout {
    dlgStretch(0);
    dlgPushButton(AWLocalise("+OK","+OK"))
      dlgAccept(1);
    dlgPushButton(AWLocalise("-Cancel", "-Abbrechen"))
      dlgReject(0);
    }
  }

AWCreateBadIndex();
