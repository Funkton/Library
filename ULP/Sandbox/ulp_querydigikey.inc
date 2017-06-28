//Author: Alex Mykyta (aim6864@rit.edu)

/////////////////////////////////////////////////////////////////////////////////////////
// THIS PROGRAM IS PROVIDED AS IS AND WITHOUT WARRANTY OF ANY KIND, EXPRESSED OR IMPLIED
/////////////////////////////////////////////////////////////////////////////////////////

// Includes function for looking up part availability and pricing from digikey

// Usage: string QueryDigikey(string ID)
// Input: Digikey Part ID
// Returns: [# Available];[Purchase QTY]@[Cost at QTY];[Purchase QTY]@[Cost at QTY];...
//
// If availability is 0, Should return "0;0@0.00000" or similar
// If ID not found or failed to parse page due to site layout changes,
// Returns "0"

string strRemoveTag(string str,char open,char close,string insert){
	// removes strings enclosed in open and close characters such as HTML tags
	// optionally inserts a string where tags existed. Only one insert in a row. no trailing inserts.
	
	// EX: str = "Hello <b>World!</b><br>"; open = '<'; close = '>'; insert = ""
	// 	Returns: "Hello World!"
	
	// EX2: str = "This <b><i>Is</b> A</i> Test!<br>"; open = '<'; close = '>'; insert = ";"
	// 	Returns: "This ;Is; A; Test!"
	
	string out;
	int selstart,selend;
	
	out = "";
	selstart = -1;
	for(int i = 0;i<strlen(str);i++){
		if(str[i] == close){
			selstart = i;
		}
		if(str[i] == open){
			selend = i;
			if(selend-selstart > 1){
				out += insert + strsub(str,selstart + 1,(selend-selstart)-1);
			}
		}
	}
	
	// grab any trailing text
	if(selend < selstart){
		if(selstart+1 < strlen(str)){
			out += insert + strsub(str,selstart+1);
		}
	}
	
	return(out);
}

string QueryDigikey(string ID){
	
	string pagetext;
	string dkURL = "http://search.digikey.com/scripts/DkSearch/dksus.dll?Detail&name=";
	int priceIDX, qtyIDX;
	
	string tbl;
	string tblArr[];
	int linecount;
	
	string linesplit[];
	string numbersplit[];
	int colcount;
	
	string out;

	int PurchaseQTY[];
	real CostAtQTY[];
	int priceentrycount = 0;
	
	int QTYAvailable;

	netget(pagetext,dkURL + ID);
	
	priceIDX = strstr(pagetext,"<tr><th>Price Break</th><th>Unit Price</th><th>Extended Price");
	qtyIDX = strstr(pagetext,"<tr><th align=right>Quantity Available</th>");

	
	if(priceIDX != -1){
		tbl = strsub(pagetext,priceIDX,strstr(pagetext,"</table>",priceIDX) - priceIDX);
		tbl = strRemoveTag(tbl,'<','>',"\t");
		linecount = strsplit(tblArr,tbl,'\n');
		
		for(int i=2;i<linecount;i++){
			colcount = strsplit(linesplit,tblArr[i],'\t');
			if(colcount == 5){
				strsplit(numbersplit,linesplit[1],',');
				PurchaseQTY[priceentrycount] = strtol(numbersplit[0] + numbersplit[1]);
				strsplit(numbersplit,linesplit[2],',');
				CostAtQTY[priceentrycount] = strtod(numbersplit[0] + numbersplit[1]);
				priceentrycount++;
			}
		}
	}
	
	if(qtyIDX != -1){
		tbl = strsub(pagetext,qtyIDX,strstr(pagetext,"</tr>",qtyIDX) - qtyIDX);
		tbl = strRemoveTag(tbl,'<','>',"");
		strsplit(tblArr,tbl,'\n');
		strsplit(numbersplit,tblArr[1],',');
		QTYAvailable = strtol(numbersplit[0] + numbersplit[1]);
		
	}
	
	// build output string
	// "QTYAvailable;PurchaseQTY@CostAtQTY;PurchaseQTY@CostAtQTY;..."
	sprintf(out,"%d",QTYAvailable);
	for(int i=0;i<priceentrycount;i++){
		sprintf(out,"%s;%d@%f",out,PurchaseQTY[i],CostAtQTY[i]);
	}
	
	return(out);
}
