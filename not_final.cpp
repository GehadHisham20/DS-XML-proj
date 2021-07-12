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
std::vector <std::string> tags_lines;

void getTags_Lines(){
    std::vector<std::string> t_l;
    tags.resize(0);
    tags_lines.resize(0);
    for(unsigned long long i=0;i<lines.size();i++){

    int tagCounter = std::count(lines[i].begin(), lines[i].end(), '<');
    int p1 = lines[i].find('<');
    int p2 = lines[i].find('>');

    for(int j=0;j<tagCounter;j++){

        tags.push_back(lines[i].substr(p1+1,p2-p1-1));
        t_l.push_back(lines[i].substr(p1+1,p2-p1-1));

        if(lines[i][p2+1] != '<'){
            int temp = lines[i].find('<',p1+1);

            t_l.push_back("~"+lines[i].substr(2+1,temp-p2-1));
        }

        int p3 = lines[i].find('<',p1+1);
        int p4 = lines[i].find('>',p2+1);
        p1 = p3;
        p2 = p4;
    }
  }
  for(unsigned long long i=0;i<t_l.size();i++){      //to remove empty lines and store tags and lines in tagsAndLines Vector
    if(! ((t_l[i][0] == '~') && (t_l[i].length() == 1)) ){
        tags_lines.push_back(t_l[i]);
    }
  }
}

struct Node{
    std::string data;
    Node* parent;
    std::vector<Node *> child;
    std::string insideData;
};

Node* newNode(std::string data){
    std::string mainTag,internalData;
        if(data.find('=') == -1){
            Node* nNode = new Node;
            nNode->data = data;
            return nNode;
        }else{
            int i = data.find(' ');
            mainTag = data.substr(0,i);
            internalData = data.substr(i+1,data.length()-1);
            Node* newNode = new Node;
            newNode->data = mainTag;
            newNode->insideData = internalData;
            return newNode;
        }
}
Node* addChild(Node* root,std::string data){
    Node* ch = newNode(data);
    root->child.push_back(ch);
    ch->parent=root;
    return ch;
}

void addChild(Node* root,Node* c){
    root->child.push_back(c);
    return;
}
Node* getLastChild(Node* root){

    if(root->child.size() == 0){
        return root;
    }
    else return getLastChild(root->child[root->child.size()-1]);

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
