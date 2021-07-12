//////////////////toka

std::vector <std::string> lines;

QFile mytempfile("out.txt");
QFile myfile("myfile.txt");

void loadFile(){

    myfile.open(QIODevice::ReadOnly |QIODevice::Text);
    std::string line;
    lines.resize(0);
    while (!myfile.atEnd())
    { line = (myfile.readLine().trimmed()).toStdString();
      lines.push_back(line);
    }

    myfile.close();

}


///////////////////////////Gehad
std::vector <std::string> lines;
std::vector <std::string> tags;
std::vector <std::string> tagsAndLines;
void getTagsAndLines(){
    std::vector<std::string> TandL;
    tags.resize(0);
    tagsAndLines.resize(0);
    for(unsigned long long x=0;x<lines.size();x++){

    int tagCounter = std::count(lines[x].begin(), lines[x].end(), '<');
    int place1 = lines[x].find('<');
    int place2 = lines[x].find('>');

    for(int m=0;m<tagCounter;m++){

        tags.push_back(lines[x].substr(place1+1,place2-place1-1));
        TandL.push_back(lines[x].substr(place1+1,place2-place1-1));

        if(lines[x][place2+1] != '<'){
            int temp = lines[x].find('<',place1+1);

            TandL.push_back("~"+lines[x].substr(place2+1,temp-place2-1));
        }

        int place3 = lines[x].find('<',place1+1);
        int place4 = lines[x].find('>',place2+1);
        place1 = place3;
        place2 = place4;
    }
  }
  for(unsigned long long x=0;x<TandL.size();x++){      //to remove empty lines and store tags and lines in tagsAndLines Vector
    if(! ((TandL[x][0] == '~') && (TandL[x].length() == 1)) ){
        tagsAndLines.push_back(TandL[x]);
    }
  }
}
//////////////////////////shaymaa
std::vector <std::string> pureTags;
std::vector <std::string> pureTagsLinesWithoutSlash;
std::vector <std::string> tags;
std::vector <std::string> tagsAndLines;

void makePureTags(){
    pureTags.resize(0);
  for(unsigned long long x=0;x<tags.size();x++){
    if(!tags[x].empty()){
        if(tags[x].find(' ') != std::string::npos){
            std::string s = tags[x].substr(0,tags[x].find(' '));
            pureTags.push_back(s);
        }else{
            pureTags.push_back(tags[x]);
        }
    }
  }
}
void makePureTagsLinesWithoutSlash(){
        pureTagsLinesWithoutSlash.resize(0);
        for(unsigned int x=0;x<tagsAndLines.size();x++){        //make tags without slash,,stored in pureTagsLinesWithoutSlash vector
            if(tagsAndLines[x][0] == '/'){
                pureTagsLinesWithoutSlash.push_back(tagsAndLines[x].substr(1,tagsAndLines[x].length()-1));
            }else{
                if(tagsAndLines[x][0] != '~'){
                    //int spacePlace = tagsAndLines[x].find(' ');
                    pureTagsLinesWithoutSlash.push_back(tagsAndLines[x]);
                }else{
                    pureTagsLinesWithoutSlash.push_back(tagsAndLines[x]);
                }
            }
    }
}
