//////////////////toka

std::vector <std::string> lines;

QFile tempxml("out.txt");
QFile myfile("myfile.txt");

void browseFile()
{

    myfile.open (QIODevice::ReadOnly |QIODevice::Text);
    std::string line;
    lines.resize(0);
    while (!myfile.atEnd())
    { 
      line = (myfile.readLine().trimmed()).toStdString();
      lines.push_back(line);
    }

    myfile.close();

}

Node* getParent(Node* root)
{
    Node* temp = root;
    if(temp->parent != NULL)
    {
       root = temp->parent;
    }
    return root;
}
Node* getMainParent(Node* root)
{

    Node* temp = root;
    while(1)
    {
        if(temp->parent == NULL)
        {
            break;
        }
        else
        {
            temp = temp->parent;
        }
    }
    return temp;
}
Node* Tree(std::vector<std::string> withoutSlash,Node* curr_root)
{
  std::stack<std::string> temp;

  for(unsigned int i=0;i<withoutSlash.size();i++)
  {    

        if(i==0)
        {
            curr_root = newNode(withoutSlash[i]);
            curr_root->parent = NULL;
            std::stringstream check(withoutSlash[i]);
            std::string s;
            getline(check, s, ' ');
            temp.push(s);
            continue;
        }

        if(withoutSlash[i][0] != '~')
        {
            std::stringstream check(withoutSlash[i]);
            std::string str;
            getline(check, str, ' ');
             if(str == temp.top())
             {
                temp.pop();
                curr_root = getParent(curr_root);
             }
            else
            {
                curr_root = addChild(curr_root,withoutSlash[i]);
                temp.push(str);
             }
        }
      else
      {
             curr_root = addChild(curr_root,withoutSlash[i].substr(1,withoutSlash[i].length()-1));
             curr_root = getParent(curr_root);
      }
  }
  return getMainParent(curr_root);
}


void OrderTraversal(Node * root)
{
    QTextStream stream(&tempxml);
    if (root==NULL)
        return;
    QList<Node *> q; 
    q.push_back(root); 
    while (!q.empty())
    {
        int i = q.size();
        while (i > 0)
        {
            Node * p = q.front();
            q.pop_front();
            stream << QString::fromStdString(p->data) << " ";
            for (unsigned long long i=0; i<p->child.size(); i++)
                q.push_back(p->child[i]);
            i--;
        }
        stream << "\n"; 
    }
    return;
}

void Quote (Node* root)
{
int p;
    for (p=0; p<root->children.size(); p++) 
{
        if (root->children[p]->data != "\*") 
{
            root->children[p]->data = "\"" + root->children[p]->data + "\"";
        }
        Quote (root->children[p]);
    }
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
///////////////////shaymaa
std::vector <std::string> tags;
std::vector <std::string> tags_lines;
std::vector <std::string> pTags;  
std::vector <std::string> pTagsWithoutSlash; 

void makePTags(){
    pTags.resize(0);
  for(unsigned long long i=0;i<tags.size();i++){
    if(!tags[i].empty()){
        if(tags[i].find(' ') != std::string::npos){
            std::string str = tags[i].substr(0,tags[i].find(' '));
            pTags.push_back(str);
        }else{
            pTags.push_back(tags[i]);
        }
    }
  }
}
void makePTagsWithoutSlash(){
        pTagsWithoutSlash.resize(0);
        for(unsigned int i=0;i<tags_lines.size();i++){        //make tags without slash,,stored in pureTagsLinesWithoutSlash vector
            if(tags_lines[i][0] == '/'){
                pTagsWithoutSlash.push_back(tags_lines[i].substr(1,tags_lines[i].length()-1));
            }else{
                if(tags_lines[i][0] != '~'){
                    pTagsWithoutSlash.push_back(tags_lines[i]);
                }else{
                    pTagsWithoutSlash.push_back(tags_lines[i]);
                }
            }
    }
}
void showChild(Node* root){
    tempxml.open(QIODevice::ReadWrite |QIODevice::Text);
    QTextStream stream(&tempxml);
    int size = root->child.size();
    for(int x=0;x<size;x++){
        stream << QString::fromStdString(root->child[x]->data)<<"\t";
    }
    stream<<"\n";
    tempxml.close();
    return;
}






bool check(Node* root){

    if(root->child.size() == 0){
        return true;
    }
    else return false;

}



void makeOneNodeForRepeatedChild(Node* root){

      if(root->child.size() < 1){
            return;
      }
      std::vector<std::string>temp;
      std::vector<std::string>rep;

     for(unsigned int x=0;x<root->child.size();x++){
        if(root->child[x]->data != "*"){
            temp.push_back(root->child[x]->data);
        }
     }

     for(unsigned int x=0;x<temp.size();x++){
        if(count(temp.begin(),temp.end(),temp[x]) > 1  && count(rep.begin(),rep.end(),temp[x]) == 0){
            rep.push_back(temp[x]);
        }
     }

     for(unsigned int i=0;i<rep.size();i++){
        Node* s = newNode(rep[i]);
        for(unsigned int y=0;y<root->child.size();y++){
            if(root->child[y]->data == rep[i]){
                root->child[y]->data = '*';
                root->child[y]->parent = s;
                addChild(s,root->child[y]);
                root->child.erase(root->child.begin()+y);
                y--;
            }

        }
        addChild(root,s);
        s->parent = root;
     }
return;
}

