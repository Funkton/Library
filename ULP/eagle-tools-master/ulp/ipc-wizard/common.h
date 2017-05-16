enum {PARA_NORMAL_REAL, PARA_NORMAL_STRING, PARA_NORMAL_INT, PARA_MINMAX, PARA_LISTBOX, PARA_DUMMY_HEADER, PARA_COMBOBOX, PARA_DUMMY_TEXT};
enum {TYPE_REAL, TYPE_REAL_MC, TYPE_INT, TYPE_INT_MC, TYPE_STRING, TYPE_STRING_MC, TYPE_TAB, TYPE_HEADER};
enum {MC_LEAST, MC_NOMINAL, MC_MOST};

string escapeString (string str) {
	string result = "";
	int k = 0;
	for (int i = 0; str[i];i++) {
		switch (str[i]) {
			case '\n':
				result[k++] = '\\';
				result[k++] = 'n';
				break;
			case '\r':
				result[k++] = '\\';
				result[k++] = 'r';
				break;
			case '\\':
				result[k++] = '\\';
				result[k++] = '\\';
				break;
			case '\'':
				result[k++] = '\\';
				result[k++] = '\'';
				break;
			default:
				result[k++] = str[i];
				break;
		}
	}
	return result;
}

string strtrim(string str) {
	string result = "";
	if (strlen(str) == 0) return "";
	int i = 0;
	while (str[i] && isspace(str[i])) {
		i++;
	}
	int s = i;
	i = strlen(str)-1;
	while (i>=0 && isspace(str[i])) {
		i--;
	}
	int e = i;
	if (e-s+1>0) return strsub(str,s,e-s+1);
	else return "";
}

string wizardName = "";
string wizardImage = "";
string wizardDescription = "";

string packageDescription = "";
string packageName = "";

int isPreview = 0;

string parameterDescription[];
string parameterIdx[];
int parameterType[];
int parameterValueInt[];
real parameterValueReal[];
string parameterValueString[];
real parameterValueMin[];
real parameterValueMax[];
int parameterValueListBox[];
string parameterListBoxEntries[];
int parameterCount = 0;

string wizardScript="";
int wizardDialogReturn = -1;

string libraryParameterIdx[];
string libraryParameterDescription[];
int libraryParameterType[];
real libraryParameterValueRealN[];
real libraryParameterValueRealM[];
real libraryParameterValueRealL[];
int libraryParameterValueIntN[];
int libraryParameterValueIntM[];
int libraryParameterValueIntL[];
string libraryParameterValueStringN[];
string libraryParameterValueStringM[];
string libraryParameterValueStringL[];
int libraryParameterCount = 0;

int saveParametersToLibrary = 0;

string ipcComponentParametersHeader = "<h3>IPC Component Parameters</h3><pre>\n";
string ipcLibraryParametersHeader = "<h3>IPC Library Parameters</h3><pre>\n";

real ipcCeil(real value, real roundOff) {
	if (roundOff == 0) return value;
	else return ceil(value/roundOff)*roundOff;
}

real ipcFloor(real value, real roundOff) {
	if (roundOff == 0) return value;
	else return floor(value/roundOff)*roundOff;
}

real ipcRound(real value, real roundOff) {
	if (roundOff == 0) return value;
	else return round(value/roundOff)*roundOff;
}



int getParameterIdx(string key) {
	for (int i=0; parameterIdx[i]; i++) {
		if (parameterIdx[i] == key) {
			return i;
		}
	}
	return -1;
}

void addParameterReal(string idx, string name, real defaultValue) {
	parameterType[parameterCount] = PARA_NORMAL_REAL;
	parameterIdx[parameterCount] = idx;
	parameterValueReal[parameterCount] = defaultValue;
	parameterDescription[parameterCount++] = name;
}

real getParameterReal(string key) {
	return parameterValueReal[getParameterIdx(key)];
}

void addParameterString(string idx, string name, string defaultValue) {
	parameterType[parameterCount] = PARA_NORMAL_STRING;
	parameterIdx[parameterCount] = idx;
	parameterValueString[parameterCount] = defaultValue;
	parameterDescription[parameterCount++] = name;
}

string getParameterString(string key) {
	return parameterValueString[getParameterIdx(key)];
}

void setParameterString(string key, string value) {
	parameterValueString[getParameterIdx(key)] = value;
}

void addParameterInt(string idx, string name, int defaultValue) {
	parameterType[parameterCount] = PARA_NORMAL_INT;
	parameterIdx[parameterCount] = idx;
	parameterValueInt[parameterCount] = defaultValue;
	parameterDescription[parameterCount++] = name;
}

int getParameterInt(string key) {
	return parameterValueInt[getParameterIdx(key)];
}

void setParameterInt(string key, int value) {
	parameterValueInt[getParameterIdx(key)] = value;
}

void addParameterMinMax(string idx, string name, real defaultMin, real defaultMax) {
	parameterType[parameterCount] = PARA_MINMAX;
	parameterIdx[parameterCount] = idx;
	parameterValueMin[parameterCount] = defaultMin;
	parameterValueMax[parameterCount] = defaultMax;
	parameterDescription[parameterCount++] = name;
}

real getParameterMin(string key) {
	return parameterValueMin[getParameterIdx(key)];
}

real getParameterMax(string key) {
	return parameterValueMax[getParameterIdx(key)];
}

void setParameterMinMax(string key, real minV, real maxV) {
	parameterValueMin[getParameterIdx(key)] = minV;
	parameterValueMax[getParameterIdx(key)] = maxV;
}

void addParameterHeader(string header) {
	parameterType[parameterCount] = PARA_DUMMY_HEADER;
	parameterIdx[parameterCount] = header;
	parameterDescription[parameterCount++] = header;
}

void addParameterText(string text) {
	parameterType[parameterCount] = PARA_DUMMY_TEXT;
	parameterIdx[parameterCount] = text;
	parameterDescription[parameterCount++] = text;
}

void addParameterComboBox(string idx, string name, string entries, int defaultValue) {
	parameterType[parameterCount] = PARA_COMBOBOX;
	parameterIdx[parameterCount] = idx;
	parameterListBoxEntries[parameterCount] = entries;
	parameterValueListBox[parameterCount] = defaultValue;
	parameterDescription[parameterCount++] = name;
}

void addParameterListBox(string idx, string name, string entries, int defaultValue) {
	parameterType[parameterCount] = PARA_LISTBOX;
	parameterIdx[parameterCount] = idx;
	parameterListBoxEntries[parameterCount] = entries;
	parameterValueListBox[parameterCount] = defaultValue;
	parameterDescription[parameterCount++] = name;
}

int getParameterListBox(string key) {
	return parameterValueListBox[getParameterIdx(key)];
}

void setParameterListBox(string key, int value) {
	parameterValueListBox[getParameterIdx(key)] = value;
}


void addParameterExposedPad(real D2, real E2, real tr, real tc, real pmc, real pmp, real pmspace) {
	addParameterHeader("Exposed Pad");
	addParameterReal("ep_D2", "Length (D2)", 0);
	addParameterReal("ep_E2", "Width (E2)", 0);
	addParameterReal("ep_tr", "Corner Radius (tr)", 0);
	addParameterReal("ep_tc", "Chamfered Corner (tc)", 0);
	addParameterReal("ep_pmc", "Paste Mask Coverage (%)", 100);
	addParameterComboBox("ep_pmp", "Paste Mask Pattern", "solid|hatched", 0);
	addParameterReal("ep_pmspace", "Paste Mask Pattern Spacing", 0.5);
}




int getLibraryParameterIdx(string key) {
	for (int i=0; libraryParameterIdx[i]; i++) {
		if (libraryParameterIdx[i] == key) {
			return i;
		}
	}
	return -1;
}

real getLibraryParameterValueReal(string key) {
	int idx = getLibraryParameterIdx(key);
	if (idx<0) dlgMessageBox("Requested key '"+key+"' not found ias Library Parameter!");
	if (libraryParameterType[idx] == TYPE_REAL_MC) {
		switch(getParameterListBox("materialCondition")) {
			case MC_LEAST: return libraryParameterValueRealL[idx];
			case MC_NOMINAL: return libraryParameterValueRealN[idx];
			case MC_MOST: return libraryParameterValueRealM[idx];
		}
	}
	return libraryParameterValueRealN[idx];
}

int getLibraryParameterValueInt(string key) {
	int idx = getLibraryParameterIdx(key);
	if (idx<0) dlgMessageBox("Requested key '"+key+"' not found ias Library Parameter!");
	if (libraryParameterType[idx] == TYPE_INT_MC) {
		switch(getParameterListBox("materialCondition")) {
			case MC_LEAST: return libraryParameterValueIntL[idx];
			case MC_NOMINAL: return libraryParameterValueIntN[idx];
			case MC_MOST: return libraryParameterValueIntM[idx];
		}
	}
	return libraryParameterValueIntN[idx];
}

string getLibraryParameterValueString(string key) {
	int idx = getLibraryParameterIdx(key);
	if (idx<0) dlgMessageBox("Requested key '"+key+"' not found ias Library Parameter!");
	if (libraryParameterType[idx] == TYPE_STRING_MC) {
		switch(getParameterListBox("materialCondition")) {
			case MC_LEAST: return libraryParameterValueStringL[idx];
			case MC_NOMINAL: return libraryParameterValueStringN[idx];
			case MC_MOST: return libraryParameterValueStringM[idx];
		}
	}
	return libraryParameterValueStringN[idx];
}

void flag(string f) {
	sprintf(wizardScript,"%s %s", wizardScript, f);
}

void setWireBend(int d) {
	sprintf(wizardScript,"%sSET WIRE_BEND %d;\n", wizardScript,d);
}

void setLayer(int d) {
	sprintf(wizardScript,"%sLAYER %d;\n", wizardScript,d);
}

void begin(string cmd) {
	wizardScript = wizardScript + cmd;
}

void position(real x, real y) {
	sprintf(wizardScript,"%s (%0.3fmm %0.3fmm)", wizardScript,x,y);
}

void curve(real c) {
	sprintf(wizardScript,"%s %+0.3f", wizardScript,c);
}

void orientation(real angle) {
	sprintf(wizardScript,"%s R%0.3f", wizardScript,angle);
}

void shape(string n) {
	sprintf(wizardScript,"%s %s", wizardScript,n);
}

void firstPad(void) {
	shape("SQUARE");
	flag("FIRST");
}

void name(string n) {
	sprintf(wizardScript,"%s '%s'", wizardScript,n);
}

void changeDrill(real s) {
	sprintf(wizardScript,"%sCHANGE DRILL %0.3fmm;", wizardScript, s);
}

real beginPad(real dmax, int rect) {
	real holeDiameter;
	if (rect)
		holeDiameter = ipcCeil(dmax+getLibraryParameterValueReal("thtHoleOverRectangularLead"),
			getLibraryParameterValueReal("thtPadSizeRoundOff"));
	else 
		holeDiameter = ipcCeil(dmax+getLibraryParameterValueReal("thtHoleOverRoundLead"),
			getLibraryParameterValueReal("thtPadSizeRoundOff"));
	real padDiameter = ipcCeil(max(holeDiameter*getLibraryParameterValueReal("thtPadToHoleRatio"),
		holeDiameter+2*getLibraryParameterValueReal("thtMinimumAnnularRing")),
		getLibraryParameterValueReal("thtPadSizeRoundOff"));

	changeDrill(holeDiameter);
	begin("PAD");
	sprintf(wizardScript,"%s %0.3fmm", wizardScript, padDiameter);
	return padDiameter;
}

void smdSize(real dx, real dy) {
	real roundOff = getLibraryParameterValueReal("cornerRadiusRoundOff");
	real minWidth = min(dx,dy);
	real cornerRadius = minWidth*getLibraryParameterValueReal("cornerRadius")/100.0;
	cornerRadius = ipcCeil(cornerRadius, roundOff);
	cornerRadius = min(cornerRadius,getLibraryParameterValueReal("cornerRadiusLimit"));
	int roundness = round (100.0*2.0*cornerRadius/minWidth);
	roundness = min(roundness, 100.0);
	
	sprintf(wizardScript,"%s %0.3fmm %0.3fmm -%d", wizardScript, dx, dy, roundness);
}

void smdSizeManual(real dx, real dy, real roundnessMm) {
	real roundOff = getLibraryParameterValueReal("cornerRadiusRoundOff");
	real minWidth = min(dx,dy);
	real cornerRadius = ipcCeil(roundnessMm, roundOff);
	int roundness = round (100.0*2.0*cornerRadius/minWidth);
	roundness = min(roundness, 100.0);
	
	sprintf(wizardScript,"%s %0.3fmm %0.3fmm -%d", wizardScript, dx, dy, roundness);
}

void end() {
	wizardScript = wizardScript + ";\n";
}

void lineWidth(real w) {
	sprintf(wizardScript,"%s %0.3fmm", wizardScript, w);
}

void beginSmd(real dx, real dy) {
	begin("SMD");
	smdSize(dx, dy);
}

void coDrawRectTerminal(real x, real y, real a, real b){
	begin("RECT");
	position(x-a/2.0,y-b/2.0);
	position(x+a/2.0,y+b/2.0);
	end();
}

void coDrawRoundTerminal(real x, real y, real d){
	real clw=getLibraryParameterValueReal("componentLineWidth");
	begin("POLYGON");
	lineWidth(clw);
	position(x-(d-clw)/2.0,y);
	curve(180);
	position(x+(d-clw)/2.0,y);
	curve(180);
	position(x-(d-clw)/2.0,y);
	end();
}

void beginCourtyard(int layer) {
	setLayer(layer);
	setWireBend(0);
	begin("WIRE");
	lineWidth(getLibraryParameterValueReal("courtyardLineWidth"));
}

void beginLegendOutline(int layer) {
	setLayer(layer);
	setWireBend(2);
	begin("WIRE");
	lineWidth(getLibraryParameterValueReal("legendLineWidth"));
}

void beginComponentOutline(int layer) {
	setLayer(layer);
	setWireBend(2);
	begin("WIRE");
	lineWidth(getLibraryParameterValueReal("componentLineWidth"));
}

void changeRatio(int percentage) {
	sprintf(wizardScript,"%sCHANGE RATIO %d;", wizardScript, percentage);
}

void changeSize(real s) {
	sprintf(wizardScript,"%sCHANGE SIZE %0.3fmm;", wizardScript, s);
}

void changeFont(string s) {
	sprintf(wizardScript,"%sCHANGE FONT %s;", wizardScript, s);
}

void changeAlign(string s) {
	sprintf(wizardScript,"%sCHANGE ALIGN %s;", wizardScript, s);
}

void generateExposedPad(string pinName, real posX, real posY) {
	int i;
	int k;
	
	real D2 = getParameterReal("ep_D2");
	real E2 = getParameterReal("ep_E2");
	if (D2 > 0 && E2 > 0) {
		real tr = getParameterReal("ep_tr");
		real tc = getParameterReal("ep_tc");
		real pmp = getParameterListBox("ep_pmp");
		real pmc = getParameterReal("ep_pmc");
		real pmspace = getParameterReal("ep_pmspace");
			
		if (tc > 0) {
			begin("SMD");
				smdSizeManual(D2 - 1.5*max(tc,tr), E2 - 1.5*max(tc,tr), tr);
				name(pinName);
				flag("NOTHERMALS");
				flag("NOCREAM");
				flag("NOSTOP");
				position(posX,posY);
			end();
			setLayer(LAYER_TOP);
			setWireBend(2);
			for (k=0; k<2; k++) {
				begin("POLYGON");
					lineWidth(tr*2);
					position(-(D2/2 - tr)+posX,(E2/2 - tr)+posY);
					position((D2/2 - tr)+posX,(E2/2 - tr)+posY);
					position((D2/2 - tr)+posX,-(E2/2 - tr)+posY);
					position(-(D2/2 - tr - tc)+posX,-(E2/2 - tr)+posY);
					position(-(D2/2 - tr)+posX,-(E2/2 - tr - tc)+posY);
					position(-(D2/2 - tr)+posX,(E2/2 - tr)+posY);
				end();
				setLayer(LAYER_TSTOP);
			}
			
		} else {
			begin("SMD");
				smdSizeManual(D2, E2, tr);
				name(pinName);
				flag("NOTHERMALS");
				if (pmc < 100) {
					flag("NOCREAM");
				}
				position(posX,posY);
			end();
		}
		
		if (pmc < 100) {
			real area = E2*D2 - 3*pow(tr,2)*(1-3.141/3) - pow(tc,2)/2;
			real pasteArea = area * pmc / 100;
			real D2v = D2 - pasteArea/E2/sqrt(pmc/100.0);
			real E2v = E2 - pasteArea/D2/sqrt(pmc/100.0);
			if ((D2v < 2*pmspace || E2v < 2*pmspace) || E2v < pmspace || D2v < pmspace|| (E2-E2v < 2*pmspace && D2-D2v < 2*pmspace)) {
				pmp = 0;
			}
			if (pmp == 0) { // SOLID
				if (tc == 0 && tr == 0) {
					setLayer(LAYER_TCREAM);
					begin("RECT");
						position(-(D2*sqrt(pmc/100.0)/2)+posX,(E2*sqrt(pmc/100.0)/2)+posY);
						position((D2*sqrt(pmc/100.0)/2)+posX,-(E2*sqrt(pmc/100.0)/2)+posY);
					end();
				} else {
					tr = max(tr,0.01);
					setLayer(LAYER_TCREAM);
					setWireBend(2);
					begin("POLYGON");
						lineWidth(tr*2);
						position(-((D2-D2v)/2 - tr)+posX,((E2-E2v)/2 - tr)+posY);
						position(((D2-D2v)/2 - tr)+posX,((E2-E2v)/2 - tr)+posY);
						position(((D2-D2v)/2 - tr)+posX,-((E2-E2v)/2 - tr)+posY);
						if (tc>0) {
							position(-((D2-D2v)/2 - tr - tc)+posX,-((E2-E2v)/2 - tr)+posY);
							position(-((D2-D2v)/2 - tr)+posX,-((E2-E2v)/2 - tr - tc)+posY);
						} else {
							position(-((D2-D2v)/2 - tr)+posX,-((E2-E2v)/2 - tr)+posY);
						}
						position(-((D2-D2v)/2 - tr)+posX,((E2-E2v)/2 - tr)+posY);
					end();
				}
			}
			
			if (pmp == 1) { // HATCHED
				real nD = floor(D2v/pmspace);
				real nE = floor(E2v/pmspace);
				
				real dSpace = D2v/nD;
				real eSpace = E2v/nE;
				real sD = (D2-D2v)/nD;
				real sE = (E2-E2v)/nE;
				
				
				real posD;
				real posE;
				setLayer(LAYER_TCREAM);
				for (i = 0; i<nD; i++) {
					posD = (i-nD/2)*(sD+dSpace) + (sD + dSpace)/2;
					for (k = 0; k<nE; k++) {
						posE = (k-nE/2)*(sE+eSpace) + (sE + eSpace)/2;
						begin("RECT");
							position(posD-sD/2+posX,posE+sE/2+posY);
							position(posD+sD/2+posX,posE-sE/2+posY);
						end();
					}
				}
			}
		}
	}
}




void loadPackageSettings() {
	int k=0;

	if (package) package(P) {
		int pos = strxstr(P.description,ipcComponentParametersHeader);
		if (pos >= 0) {
			string parameters[];
			int n = strsplit(parameters,strsub(P.description, pos), '\n');
			for (int i=1; i<n; i++) {
				if (parameters[i] == "</pre>") break;
				string cell[];
				int m = strsplit(cell,parameters[i], '|');
				if (m>1) {
					int idx = getParameterIdx(strtrim(cell[0]));
					switch (parameterType[idx]) {
						case PARA_NORMAL_REAL:
							parameterValueReal[idx] = strtod(cell[1]);
							break;
						case PARA_NORMAL_INT:
							parameterValueInt[idx] = strtol(cell[1]);
							break;
						case PARA_NORMAL_STRING:
							parameterValueString[idx] = strtrim(cell[1]);
							break;
						case PARA_MINMAX:
							parameterValueMin[idx] = strtod(cell[1]);
							parameterValueMax[idx] = strtod(cell[2]);
							break;
						case PARA_COMBOBOX:
							parameterValueListBox[idx] = strtol(cell[1]);
							break;
						case PARA_LISTBOX:
							parameterValueListBox[idx] = strtol(cell[1]);
							break;
						default:
							break;
					}
					int type = parameterType[idx];
				}
			}
		}
	}
}

string generatePackageDescription () {
	string elements[];
	string strData = ipcComponentParametersHeader;
	for (int i=0; i<parameterCount; i++) {
		switch (parameterType[i]) {
			case PARA_NORMAL_REAL:
				sprintf(strData, "%s%s | %f\n", strData, parameterIdx[i], parameterValueReal[i]);
				break;
			case PARA_NORMAL_INT:
				sprintf(strData, "%s%s | %d\n", strData, parameterIdx[i], parameterValueInt[i]);
				break;
			case PARA_NORMAL_STRING:
				sprintf(strData, "%s%s | %s\n", strData, parameterIdx[i], parameterValueString[i]);
				break;
			case PARA_MINMAX:
				sprintf(strData, "%s%s | %f | %f\n", strData, parameterIdx[i], parameterValueMin[i], parameterValueMax[i]);
				break;
			case PARA_COMBOBOX:
				strsplit(elements,parameterListBoxEntries[i],'|');
				sprintf(strData, "%s%s | %d | %s\n", strData, parameterIdx[i], parameterValueListBox[i], elements[parameterValueListBox[i]]);
				break;
			case PARA_LISTBOX:
				strsplit(elements,parameterListBoxEntries[i],'|');
				sprintf(strData, "%s%s | %d | %s\n", strData, parameterIdx[i], parameterValueListBox[i], elements[parameterValueListBox[i]]);
				break;
			default:
				break;
		}
	}
	return strData+"\n</pre>\n";
}

string generateEditedBy () {
	return "\nPackage generated by: "+filename(argv[0]);
}

string generateLibraryDescription() {
	string strData = ipcLibraryParametersHeader;
	for (int i=0; i<libraryParameterCount; i++) {
		switch (libraryParameterType[i]) {
			case TYPE_REAL:
				sprintf(strData, "%s%s | %f\n", strData, libraryParameterIdx[i], libraryParameterValueRealN[i]);
				break;
			case TYPE_INT:
				sprintf(strData, "%s%s | %d\n", strData, libraryParameterIdx[i], libraryParameterValueIntN[i]);
				break;
			case TYPE_STRING:
				sprintf(strData, "%s%s | %s\n", strData, libraryParameterIdx[i], libraryParameterValueStringN[i]);
				break;
			case TYPE_REAL_MC:
				sprintf(strData, "%s%s | %f | %f | %f\n", strData, libraryParameterIdx[i], libraryParameterValueRealL[i], libraryParameterValueRealN[i], libraryParameterValueRealM[i]);
				break;
			case TYPE_INT_MC:
				sprintf(strData, "%s%s | %d | %d | %d\n", strData, libraryParameterIdx[i], libraryParameterValueIntL[i], libraryParameterValueIntN[i], libraryParameterValueIntM[i]);
				break;
			case TYPE_STRING_MC:
				sprintf(strData, "%s%s | %s | %s | %s\n", strData, libraryParameterIdx[i], libraryParameterValueStringL[i], libraryParameterValueStringN[i], libraryParameterValueStringM[i]);
				break; 
		}
	}
	return strData+"\n</pre>\n";
}

void loadLibraryFromDescription (string description) {
	int pos = strxstr(description,ipcLibraryParametersHeader);
	if (pos >= 0) {
		string parameters[];
		int n = strsplit(parameters,strsub(description, pos), '\n');
		for (int i=1; i<n; i++) {
			if (parameters[i] == "</pre>") break;
			string cell[];
			int m = strsplit(cell,parameters[i], '|');
			if (m>=2) {
				int idx = getLibraryParameterIdx(strtrim(cell[0]));
				if (idx >= 0) {
					switch (libraryParameterType[idx]) {
						case TYPE_REAL:
							libraryParameterValueRealN[idx] = strtod(cell[1]);
							break;
						case TYPE_INT:
							libraryParameterValueIntN[idx] = strtol(cell[1]);
							break;
						case TYPE_STRING:
							libraryParameterValueStringN[idx] = strtrim(cell[1]);
							break;
						case TYPE_REAL_MC:
							libraryParameterValueRealL[idx] = strtod(cell[1]);
							libraryParameterValueRealN[idx] = strtod(cell[2]);
							libraryParameterValueRealM[idx] = strtod(cell[3]);
							break;
						case TYPE_INT_MC:
							libraryParameterValueIntL[idx] = strtol(cell[1]);
							libraryParameterValueIntN[idx] = strtol(cell[2]);
							libraryParameterValueIntM[idx] = strtol(cell[3]);
							break;
						case TYPE_STRING_MC:
							libraryParameterValueStringL[idx] = strtrim(cell[1]);
							libraryParameterValueStringN[idx] = strtrim(cell[2]);
							libraryParameterValueStringM[idx] = strtrim(cell[3]);
							break;
					}
				}
			}
		}
	}
}

void loadLibraryDefaultSettings() {
	string settingsFilename = filedir(argv[0]) + "libraryParameters.data";
	string lines[];
	int n = fileread(lines, settingsFilename);
	for (int i=0; i<n; i++) {
		string cell[];
		int m = strsplit(cell, lines[i], '\t');
		if (m>=1) {
			string ident = strtrim(cell[0]);
			if (ident[0] == '#') {
				if (ident == "#Tab") {
					libraryParameterIdx[libraryParameterCount] = ident;
					libraryParameterType[libraryParameterCount] = TYPE_TAB;
					libraryParameterDescription[libraryParameterCount] = cell[1];
				}
				if (ident == "#Header") {
					libraryParameterIdx[libraryParameterCount] = ident;
					libraryParameterType[libraryParameterCount] = TYPE_HEADER;
					libraryParameterDescription[libraryParameterCount] = cell[1];
				}
			} else {
				if (m>=4) {
					libraryParameterIdx[libraryParameterCount] = ident;
					libraryParameterDescription[libraryParameterCount] = cell[1];
					if (m == 4) {
						if (cell[2] == "real") {
							libraryParameterType[libraryParameterCount] = TYPE_REAL;
							libraryParameterValueRealN[libraryParameterCount] = strtod(cell[3]);
						}
						if (cell[2] == "int") {
							libraryParameterType[libraryParameterCount] = TYPE_INT;
							libraryParameterValueIntN[libraryParameterCount] = strtol(cell[3]);
						}
						if (cell[2] == "string"){
							libraryParameterType[libraryParameterCount] = TYPE_STRING;
							libraryParameterValueStringN[libraryParameterCount] = cell[3];
						} 
					}
					
					if (m == 6) {
						if (cell[2] == "real") {
							libraryParameterType[libraryParameterCount] = TYPE_REAL_MC;
							libraryParameterValueRealL[libraryParameterCount] = strtod(cell[3]);
							libraryParameterValueRealN[libraryParameterCount] = strtod(cell[4]);
							libraryParameterValueRealM[libraryParameterCount] = strtod(cell[5]);
						}
						if (cell[2] == "int") {
							libraryParameterType[libraryParameterCount] = TYPE_INT_MC;
							libraryParameterValueIntL[libraryParameterCount] = strtol(cell[3]);
							libraryParameterValueIntN[libraryParameterCount] = strtol(cell[4]);
							libraryParameterValueIntM[libraryParameterCount] = strtol(cell[5]);
						}
						if (cell[2] == "string"){
							libraryParameterType[libraryParameterCount] = TYPE_STRING_MC;
							libraryParameterValueStringL[libraryParameterCount] = cell[3];
							libraryParameterValueStringN[libraryParameterCount] = cell[4];
							libraryParameterValueStringM[libraryParameterCount] = cell[5];
						} 
					}
					
					libraryParameterDescription[libraryParameterCount] = cell[1];
				}
			}
			libraryParameterCount++;
		}
	}
}

void loadComponentDefaultSettings() {
	int k=0;
	string materialCondition[];
	materialCondition[k++] = "Least (High Density)";
	materialCondition[k++] = "Nominal (Nominal Density)";
	materialCondition[k++] = "Most (Low Density)";

	addParameterComboBox("materialCondition", "Material Condition", strjoin(materialCondition,'|'), MC_NOMINAL);
}

loadLibraryDefaultSettings();
loadComponentDefaultSettings();

void runWizard() {
	if (library) library(L) loadLibraryFromDescription(L.description);
	if (argc>1) {
		if (argv[1] == "EDIT") {
			loadPackageSettings();
			if (package) package(P) loadLibraryFromDescription(P.description);
		}
	}
	wizardDialogReturn = dlgDialog("IPC Footprint Wizard: "+ wizardName) {
		dlgLabel("<table bgcolor=\"#ffffff\" width=\"100%\" height=70 cellpadding=\"10\"><tr><td align=\"left\"><font size=5>" + wizardName + "</font><img align=\"right\" height=48 src=\""+ wizardImage +"\"><br><br>Enter footprint parameter</td></tr></table>");
		dlgHBoxLayout {
			
			dlgTabWidget {
				dlgTabPage ("Component") {
					dlgHBoxLayout {
						dlgGridLayout {
							int lastWasMinMax = 0;
							int k = 0;
							
							

							
							for (int i; parameterDescription[i]; i++) {
								//dlgCell(k, 0) dlgStretch(1);
								dlgCell(k, 3) dlgStretch(1);
								switch (parameterType[i]) {
									case PARA_DUMMY_TEXT:
										dlgCell(k, 0, k, 2) {
												dlgSpacing(10);
												dlgLabel("<p>"+parameterDescription[i]+"</p>");
										}
										
										break;
									case PARA_DUMMY_HEADER:
										
										dlgCell(k, 0, k, 2) dlgVBoxLayout {
												dlgSpacing(5);
												dlgLabel("<b>"+parameterDescription[i]+"</b><hr>");
										}
										break;
									default:
										if (parameterType[i] == PARA_MINMAX && lastWasMinMax == 0) {
											dlgCell(k, 1) {
												dlgLabel("<p align=\"left\"><font size=2>Minimum</font></p>");
												dlgStretch(1);
												//dlgLabel("<table width=100%><tr><td align=\"center\"><font size=2>Minimum</font></td><td align=\"center\"><font size=2>Maximum</font></td></tr></table>");
											};
											dlgCell(k, 1) {
												dlgStretch(1);
												dlgLabel("<p align=\"right\"><font size=2>Maximum</font></p>");
												//dlgLabel("<table width=100%><tr><td align="center"><font size=2>Minimum</font></td><td align="center"><font size=2>Maximum</font></td></tr></table>");
											};
											//dlgCell(k, 3) dlgLabel("<p align=\"center\">Maximum</p>");
											k++;
										}
										dlgCell(k, 0) {
											dlgSpacing(10);
											dlgLabel("<p>"+parameterDescription[i]+"</p>");
										}
										break;
								}
								
								
								switch (parameterType[i]) {
									case PARA_NORMAL_REAL:
										dlgCell(k, 1) {
											dlgRealEdit(parameterValueReal[i]);
											//dlgStretch(1);
										};
										break;
									case PARA_NORMAL_INT:
										dlgCell(k, 1) {
											dlgIntEdit(parameterValueInt[i]);
											//dlgStretch(1);
										}
										break;
									case PARA_NORMAL_STRING:
										dlgCell(k, 1) {
											dlgStringEdit(parameterValueString[i]);
											//dlgStretch(1);
										};
										break;
									case PARA_MINMAX:
										
										dlgCell(k,1) {
											dlgRealEdit(parameterValueMin[i]);
											dlgStretch(1);
										}
										dlgCell(k,1) {
											dlgStretch(1);
											dlgRealEdit(parameterValueMax[i]);
										}
										//dlgCell(k,3) dlgRealEdit(parameterValueMax[i]);
										break;
									case PARA_COMBOBOX:
										string listBoxEntries[];
										strsplit(listBoxEntries,parameterListBoxEntries[i],'|');
										
										dlgCell(k, 1, k, 2) {
											dlgComboBox(listBoxEntries,parameterValueListBox[i]);
											dlgStretch(1);
											
										}
										break;
									default:
										break;
								}
								if (parameterType[i] == PARA_MINMAX) {
									lastWasMinMax = 1;
								} else {
									lastWasMinMax = 0;
								}
								
								k++;
							}
							/*
							dlgCell(0, 5) dlgSpacing(800);
							dlgCell(0, 6) dlgStretch(1);
							dlgCell(0, 7) dlgStretch(1);
							dlgCell(0, 8) dlgStretch(1);
							dlgCell(0, 9) dlgStretch(1);
							
							dlgCell(0, 6, k, 8) {
								
							}
							*/
						}
						
					}
					dlgStretch(1);
				}
				
				dlgTabPage ("Library Settings") {
					dlgHBoxLayout {
						dlgTabWidget {
							for (int i = 0; i<libraryParameterCount; i++) {
								if (libraryParameterType[i] == TYPE_TAB) {
									dlgTabPage (libraryParameterDescription[i]) {
										dlgGridLayout {
											dlgCell(0, 1) {
												dlgLabel("<p align=\"center\">Material Condition</p><table width=100%><tr><td align=\"left\">Least</td><td align=\"center\">Nominal</td><td align=\"right\">Most</td></tr></table>");
											};
											int l = 1;
											for (i = i + 1; i<libraryParameterCount; i++) {
												
												if (libraryParameterType[i] == TYPE_TAB) {
													i--;
													break;
												}
												switch (libraryParameterType[i]) {
													case TYPE_HEADER:
														dlgCell(l, 0, l, 3) dlgVBoxLayout {
															dlgSpacing(5);
															dlgLabel("<b>"+libraryParameterDescription[i]+"</b><hr>");
														}
														break;
													default:
														dlgCell(l, 0) {
															dlgSpacing(10);
															dlgLabel("<p>"+libraryParameterDescription[i]+"</p>");
														}
														break;
												}
												
												switch (libraryParameterType[i]) {
													case TYPE_REAL:
														dlgCell(l, 1) dlgRealEdit(libraryParameterValueRealN[i]);
														break;
													case TYPE_INT:
														dlgCell(l, 1) dlgIntEdit(libraryParameterValueIntN[i]);
														break;
													case TYPE_STRING:
														dlgCell(l, 1) dlgStringEdit(libraryParameterValueStringN[i]);
														break;
													case TYPE_REAL_MC:
														dlgCell(l, 1) {
															dlgRealEdit(libraryParameterValueRealL[i]);
															dlgStretch(2);
														};
														dlgCell(l, 1) {
															dlgStretch(1);
															dlgRealEdit(libraryParameterValueRealN[i]);
															dlgStretch(1);
														}
														dlgCell(l, 1) {
															dlgStretch(2);
															dlgRealEdit(libraryParameterValueRealM[i]);
														}
														break;
													case TYPE_INT_MC:
														dlgCell(l, 1) {
															dlgIntEdit(libraryParameterValueIntL[i]);
															dlgStretch(2);
														}
														dlgCell(l, 1) {
															dlgStretch(1);
															dlgIntEdit(libraryParameterValueIntN[i]);
															dlgStretch(1);
														}
														dlgCell(l, 1) {
															dlgStretch(2);
															dlgIntEdit(libraryParameterValueIntM[i]);
														}
														break;
													case TYPE_STRING_MC:
														dlgCell(l, 1) {
															dlgStringEdit(libraryParameterValueStringL[i]);
															dlgStretch(2);
														}
														dlgCell(l, 1) {
															dlgStretch(1);
															dlgStringEdit(libraryParameterValueStringN[i]);
															dlgStretch(1);
														}
														dlgCell(l, 1) {
															dlgStretch(2);
															dlgStringEdit(libraryParameterValueStringM[i]);
														}
														break;
													default:
														break;
												}
												l++;
											}
										}
										dlgStretch(1);
									}
								}
							}
						}
						
						//dlgStretch(1);
					}
					
					//dlgStretch(1);
					dlgCheckBox("Save as library defaults", saveParametersToLibrary);

					dlgPushButton("Load from Library") {
						if (library) library(L) loadLibraryFromDescription(L.description);
					}
					dlgPushButton("Load from Component") {
						if (package) package(P) loadLibraryFromDescription(P.description);
					}
				}
				
				
			}
			//dlgSpacing(5);
			dlgVBoxLayout {
				dlgStretch(1);
				dlgLabel(wizardDescription,1);
				dlgStretch(1);
			}
			
			
		}
		dlgLabel("<hr>");
		//dlgSpacing(5);
		dlgHBoxLayout {
			dlgStretch(1);
			
			dlgPushButton("< Back") {
				isPreview = 1;
				dlgReject(-2);
			}
			dlgPushButton("&Preview") {
				isPreview = 1;
				dlgAccept(1);
			}
			dlgPushButton("+&Finish >") {
				dlgAccept(2);
			}
			dlgSpacing(8);
			dlgPushButton("&Cancel") {
				dlgReject(-1);
			}
		}
	};
	if (wizardDialogReturn < 1) {
		if (wizardDialogReturn == -2) {
			string currentDir = filedir(argv[0]);
			exit("RUN '" + filedir(argv[0]) + "../ipc-wizard.ulp';");
		} else {
			exit("");
		}
	}
}

real calcZmax(real Lmin, real Lmax, real Jt) {
	real F = getLibraryParameterValueReal("fabTol");
	real P = getLibraryParameterValueReal("placeTol");
	real Ltol = Lmax - Lmin;
	return (Lmin + 2.0*Jt + sqrt(Ltol*Ltol + 4.0*F*F + 4.0*P*P));
}

real calcGmin(real Lmin, real Lmax, real Tmin, real Tmax, real Jh) {
	real F = getLibraryParameterValueReal("fabTol");
	real P = getLibraryParameterValueReal("placeTol");
	real Ltol = Lmax - Lmin;
	real Ttol = Tmax - Tmin;
	real Stol = Ltol + 2.0*Ttol;
	real StolRMS = sqrt(Ltol*Ltol + 2.0*Ttol*Ttol);
	real Sdiff = Stol - StolRMS;
	real Smin = Lmin - 2.0*Tmax + Sdiff/2.0;
	real Smax = Lmax - 2.0*Tmin - Sdiff/2.0;
	return (Smax - 2.0*Jh - sqrt(StolRMS*StolRMS + 4.0*F*F + 4.0*P*P));
}

real calcXmax(real Wmin, real Wmax, real Js) {
	real F = getLibraryParameterValueReal("fabTol");
	real P = getLibraryParameterValueReal("placeTol");
	real Wtol = Wmax - Wmin;
	return (Wmin + 2.0*Js + sqrt(pow(Wmax-Wmin,2) + 4.0*F*F + 4.0*P*P));
}

void exitWizard() {
	string preamble = "";
	sprintf(preamble,"EDIT %s.pac;\n",packageName);
	
	if (isPreview == 1) {
		preamble = "EDIT ipc-preview-dummy.pac;\n";
	}
	preamble = preamble + "GRID MM; GRID 0.01;\n";
	preamble = preamble + "RUN '" + filedir(argv[0]) + "util/clear-package.ulp';\n";
	
	wizardScript = preamble + wizardScript;

	wizardScript = wizardScript + "DESCRIPTION '" + escapeString(packageDescription) + escapeString(generatePackageDescription()) + escapeString(generateLibraryDescription()) + escapeString(generateEditedBy()) + "';";
	wizardScript = wizardScript + "WINDOW FIT;\n";
	if (saveParametersToLibrary==1) {	
		if (library) library(L) {
			string desc = L.description;
			int pos = strxstr(desc,ipcLibraryParametersHeader);
			if (pos >= 0) {
				desc = strsub(L.description,0,pos);
			}
			wizardScript = wizardScript + "DESCRIPTION * '" + escapeString(desc) + escapeString(generateLibraryDescription()) + "';";
		}
	}
	if (wizardDialogReturn == 1) { // PREVIEW
		wizardScript = wizardScript + "RUN '" + argv[0] +"' EDIT;\n";
	}
	
	exit(wizardScript);
}