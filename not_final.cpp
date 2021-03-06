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
            for (unsigned long long j=0; j<p->child.size(); j++)
                q.push_back(p->child[j]);
            i--;
        }
        stream << "\n"; 
    }
    return;
}

void organizeTree (Node* root)
{
if (root==NULL)
{
    return ;
}
makeOneNodeForRepeatedChild (root);
for (unsigned int i=0;i<root->child.size();i++)
{
       organizeTree (root->child[i]);
}
return;
}

void Quote (Node* root)
{
    for (unsigned int p=0; p<root->child.size(); p++) 
{
        if(root->child[p]->data != "*"&&!check(root->child[p]))
{
            root->child[p]->data = "\"" + root->child[p]->data + "\"";
        }
        Quote (root->child[p]);
    }
}


void Brackets(Node* root)
{

    for (unsigned int y=0; y<root->child.size(); y++)
    {

        bool case1 = (root->child.size() >= 1) && (root->child[0]->data != "*") && (root->child[y]->child.size() != 0) && (y == root->child.size()-1);

        bool case2 = (root->child.size() > 1) && (root->child[0]->data == "*") && (y == root->child.size()-1);

        if (case1)
        {
            Node* temp1 = getLastChild(root);
            if (temp1->data[temp1->data.length()-1] == ']' || (temp1->data[temp1->data.length()-1] == '}') )
            {
                int o = count(temp1->data.begin(),temp1->data.end(),']');
                int p = count(temp1->data.begin(),temp1->data.end(),'}');
                if(o<0){o=0;}
                if(p<0){p=0;}
                int s1 = o+p;
                temp1->data.insert(temp1->data.length()-s1,"}");
            }
            else
            {
                temp1->data = temp1->data + "}";
            }
        }
        else if (case2)
        {
            Node* temp2 = getLastChild(root->child[y]);
            if (temp2->data[temp2->data.length()-1] == ']' || (temp2->data[temp2->data.length()-1] == '}') )
            {
                int k = count(temp2->data.begin(),temp2->data.end(),']');
                int l = count(temp2->data.begin(),temp2->data.end(),'}');
                if(k<0){k=0;}
                if(l<0){l=0;}
                int s2 = k+l;
                temp2->data.insert(temp2->data.length()-s2,"]");
            }
            else
            {
                temp2->data = temp2->data + "]";
            }
        }
       Brackets(root->child[y]);
    }
}

void printNode (Node* root)
{
    
   if (root->data == "\*" && root->child.size() == 1 && root->child[0]->child.size() == 0 && root->insideData.empty()) {
                return;
            }if (root->data == "\*" && root->child.size() == 1 && root->child[0]->child.size() == 0 && !root->insideData.empty()) {
                json += root->insideData;
            }
            else if (root->data == "\*" && root->child.size() >= 1 && root->insideData.empty()) {
                json += "{";
            }
            else if (root->data == "\*" && root->child.size() >= 1 && !root->insideData.empty()) {
                json += root->insideData;
            }
            else if (root->child.size() == 0 && (root->data[root->data.length() - 1] == '}' || root->data[root->data.length() - 1] == ']')) {
                if (!root->parent->insideData.empty()) {
                    json += "\"text\"\:" + root->data + ",";
                }
                else { json += root->data + ","; }
            }
            else if (root->child.size() == 0) {
                if (root->parent->child.size() == 1 && root->parent->insideData.empty()) {
                    json += root->data + ",";
                }
                else { json += root->data + "},"; }
            }
            else if (root->child.size() == 1 && root->child[0]->child.size() != 0 && root->data != "\*" && root->insideData.empty() && root->parent != NULL) {
                json += root->data + "\:{";
            }
            else if (root->child.size() == 1 && root->child[0]->child.size() != 0 && root->data != "\*" && !root->insideData.empty() && root->parent != NULL) {
                json += root->data + "\:" + root->insideData;
            }
            else if (root->child.size() == 1 && root->data != "\*" && root->parent != NULL && root->insideData.empty()) {
                json += root->data + ":";
            }
            else if (root->child.size() == 1 && root->data != "\*" && root->parent != NULL && !root->insideData.empty()) {
                json += root->data + "\:" + root->insideData;
            }
            else if (root->child.size() >0 && root->child[0]->data == "\*") {
                json += root->data + "\:[";
            }
            else if (root->child.size() > 0 && root->child[0]->data != "\*" && root->insideData.empty()) {
                json += root->data + "\:";
            }
            else if (root->child.size() > 0 && root->child[0]->data != "\*" && !root->insideData.empty()) {
                json += root->data + "\:" + root->insideData;
            }
}


void print(Node* root)
{

    printNode(root);
    for(unsigned int z=0;z<root->child.size();z++)
    {
        print(root->child[z]);
    }
    return;
}

void correctError() 
{
  if (Cases.size()>0)
  {
   for(unsigned int i=0;i<errors.size();i++)
   {
    if (Cases[i] == 2)
    {
        std::string str1;
        std::stringstream c1 (lines[errors[i]-1]);
        getline(c1, lines[errors[i]-1] , '/');
        std::string temp = lines[errors[i]-1].substr(0,lines[errors[i]-1].length()-1);
        std::stringstream check2( temp );
        getline(check2, str1 , '>');
        lines[errors[i]-1] = temp + "</" + str1.substr(1,str1.find_first_of(' ')) +">";

    }
    if (Cases[i] == 1 && lines[errors[i]-1][lines[errors[i]-1].length()-1] != '>')
    {
        std::string str2;
        std::stringstream c2( lines[errors[i]-1] );
        getline(c2, str2 , '>');
        lines[errors[i]-1] = lines[errors[i]-1] + "</" + str2.substr(1,str2.find_first_of(' ')) +">";
    }
    else if (Cases[i]==1)
    {
        std::string str3;
        std::stringstream c3( lines[errors[i]-1] );
        getline(c3, str3 , '>');
        for(unsigned int j=0;j<lines.size();j++)
        {
            if(lines[j].empty())
            {
                lines[j] = "</" + str3.substr(1,str3.find_first_of(' ')) +">";
            }
        }
    }

   }
  }
  return;
}

void MainWindow::on_Minify_Button_clicked()
{
    ui->output_text->clear();
    ui->output_text->setLineWrapMode(QPlainTextEdit::LineWrapMode::WidgetWidth);
          QFile tagsfile("mytags.txt");
          tagsfile.resize(0);
          tempxml.resize(0);
             generatefile(&myfile,&tagsfile);


             tagsfile.open(QIODevice::ReadWrite |QIODevice::Text);
             tempxml.open(QIODevice::ReadWrite |QIODevice::Text);
             QTextStream str(&tempxml);
             QString word;

             while (!tagsfile.atEnd())
             { word = tagsfile.readLine().trimmed();
                if(word.isEmpty()){continue;}
                  str<<word;
             }
             tempxml.close();
             tempxml.open(QIODevice::ReadWrite |QIODevice::Text);
             QTextStream strq(&tempxml);
             ui->output_text->setPlainText(strq.readAll());
             tempxml.close();
             tagsfile.close();

}

void MainWindow::on_Check_Button_clicked()
{   if(errors.size() == 0)
    {
     QMessageBox error1;

     error1.setText("XML File is Correct");
     error1.exec();
     return;
    }
    else
    {
    lines.resize(0);
    ui->output_text->clear();
    std::string line;
    QTextCharFormat format;
    QTextCursor cursor( ui->output_text->textCursor() );
     browseFile();
     findError();
     int j =0;
       for (unsigned int i=1;i<lines.size()+1;i++)
       {
           line=lines[i-1];
           if (i==errors[j])
           {
               format.setFontWeight( QFont::TypeWriter );
               format.setForeground( QBrush( QColor(Qt::red) ) );
               cursor.setCharFormat( format );
               cursor.insertText(QString::fromStdString(line));
               if(cursor.PreviousCharacter != '\n'){cursor.insertText("\n");}
               j++;
           }
           else
           {
               format.setFontWeight( QFont::TypeWriter );
               format.setForeground( QBrush( QColor(Qt::black) ) );
               cursor.setCharFormat( format );
               cursor.insertText(QString::fromStdString(line));
               if(cursor.PreviousCharacter != '\n'){cursor.insertText("\n");}
           }
       }
     }

return;
}

void MainWindow::on_Correct_Button_clicked()
{
    if(errors.size() == 0)
        {
         QMessageBox error2;
         error2.setText("XML File is Correct");
         error2.exec();
         return;
        }
    else
    {
        correctError();
        ui->output_text->clear();
        std::string line;
        QTextCharFormat format;
        QTextCursor cursor( ui->output_text->textCursor() );
           int j =0;
           for (unsigned int i=1;i<lines.size()+1;i++)
           {line=lines[i-1];
               if(i == errors[j])
               {
                   format.setFontWeight( QFont::TypeWriter );
                   format.setForeground( QBrush( QColor(Qt::darkGreen) ) );
                   cursor.setCharFormat( format );
                   cursor.insertText(QString::fromStdString(line));
                   cursor.insertText("\n");
                   j++;
               }
               else
               {
                   format.setFontWeight( QFont::TypeWriter );
                   format.setForeground( QBrush( QColor(Qt::black) ) );
                   cursor.setCharFormat( format );
                   cursor.insertText(QString::fromStdString(line));
                   cursor.insertText("\n");
               }
           }
         }
}

///////////////////////////Gehad////////////////////////////////////////////
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

#include "mainwindow.h"
#include "ui_mainwindow.h"

void generatefile(QFile *input,QFile *output){
    (*input).open(QIODevice::ReadWrite |QIODevice::Text);
    (*output).open(QIODevice::ReadWrite |QIODevice::Text);
    QTextStream stream(output);
    QString l;
    QChar t;

    while (!myfile.atEnd())
    { l = myfile.readLine();

        for (auto o:l)
        {
           if(o=='\n'||o=='\t'){t=o;continue;}
           if((o=='<')&&(t=='\n')){stream<<o;t=o;continue;}
           if((o=='<')&&(t=='>')){stream<<'\n'<<o;t=o;continue;}
           if(o=='>'){stream<<o<<'\n';t=o;continue;}
           stream<<o;
        }
    }

    (*input).close();
    (*output).close();
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_Browse_Button_clicked()
{
    ui->input_text->clear();
    QFile input_file(QFileDialog::getOpenFileName(this,tr("Open File"),"",tr("XML File (*.xml) ;;TextFile (*.txt)")));
    input_file.open(QIODevice::ReadOnly |QIODevice::Text);
    QTextStream stream(&input_file);
    QString text= stream.readAll();
    myfile.remove();
    tempxml.resize(0);
    input_file.copy("myfile.txt");
    QFile myfile("myfile.txt");
    ui->input_text->setPlainText(text);
    ui->input_text->setLineWrapMode(QPlainTextEdit::NoWrap);
   input_file.close();
}
void MainWindow::on_Save_Button_clicked()
{
 QFile output_file(QFileDialog::getSaveFileName(this,tr("Save File"),"",tr("Text File ()*.txt;;XML File ()*.xml")));
 output_file.open(QIODevice::ReadWrite|QIODevice::Text);
 QString text=ui->output_text->toPlainText();
     output_file.write(text.toUtf8());
     output_file.close();
}
void MainWindow::on_JSON_Button_clicked()
{    tempxml.resize(0);
     tempxml.open(QIODevice::ReadWrite |QIODevice::Text);
     QTextStream st(& tempxml);

      browseFile();   

      getTags_Lines();      

      makePTags();         

      makePTagsWithoutSlash();              

      Node* curr = NULL;

      curr = Tree(pTagsWithoutSlash,curr);      

      organizeTree(curr);                                         

      toJson(curr);                       

      st<<QString::fromStdString(json);        
      tempxml.close();
      tempxml.open(QIODevice::ReadWrite |QIODevice::Text);
      QTextStream strq(&tempxml);
      ui->output_text->setLineWrapMode(QPlainTextEdit::LineWrapMode::NoWrap);
      ui->output_text->clear();
      QString out=strq.readAll();
      QChar prev;
       int l=0;
       for(auto o:out)
      {
          if(o=='{'){
              ui->output_text->insertPlainText(o);
              ui->output_text->insertPlainText("\n");
              l=l+1;
          }
          else if(o=='}'){

              ui->output_text->insertPlainText("\n");
              ui->output_text->insertPlainText(o);
              l=l-1;}
          else if(o=='['){
              ui->output_text->insertPlainText(o);
              ui->output_text->insertPlainText("\n");
              l=l+1;}
          else if(o==']'){

              ui->output_text->insertPlainText("\n");
               ui->output_text->insertPlainText(o);
               l=l-1;}
          else if(o==',')
          {
              ui->output_text->insertPlainText(o);
              ui->output_text->insertPlainText("\n");
          }
          else {ui->output_text->insertPlainText(o);}
          prev=o;
      }
      tempxml.close();
      json = "{";
      lines.resize(0);
       tags.resize(0);
       tags_lines.resize(0);
       pTags.resize(0);
      pTagsWithoutSlash.resize(0);
       return;
}
void MainWindow::on_Check_Button_clicked()
{}
void MainWindow::on_Minify_Button_clicked()  
{}

void MainWindow::on_Correct_Button_clicked()
{}


void MainWindow::on_Prettify_Button_clicked()
{
    QTextCursor cursor( ui->output_text->textCursor() );
    QTextCharFormat format;
    format.setFontWeight( QFont::TypeWriter );



       ui->output_text->clear();
       ui->output_text->setLineWrapMode(QPlainTextEdit::NoWrap);
       QFile tagsfile("mytags.txt");
        tagsfile.resize(0);
          generatefile(&myfile,&tagsfile);
        tempxml.resize(0);
        tempxml.open(QIODevice::ReadWrite |QIODevice::Text);
        QTextStream str(&tempxml);
        tagsfile.open(QIODevice::ReadOnly |QIODevice::Text);

             QString word,wordpre;
             int l = 0;
             int curr,pre;
             int out=ui->input_text->blockCount();
    if(out<8000){
             while (!tagsfile.atEnd())
          {
            format.setForeground( QBrush( QColor(Qt::blue) ) );
             word = tagsfile.readLine().trimmed();
             if(word.isEmpty()){continue;}
             curr=classification(word);
             pre=classification(wordpre);

          //opening
              if(curr==1){
                  if(pre==0)
                  {
                  cursor.setCharFormat( format );
                  cursor.insertText( word );
                  l=l+1;
                  }
                  else if(pre==1)
                  {
                  cursor.insertText("\n");
                  for(int i=0;i<l;i++){cursor.insertText("  ");}
                  cursor.setCharFormat( format );
                  cursor.insertText( word );
                  l=l+1;
                  }
                  else
                  {
                  cursor.insertText("\n");
                  for(int i=0;i<l;i++){cursor.insertText("  ");}
                  cursor.setCharFormat( format );
                  cursor.insertText( word );
                  l=l+1;
                  }
              }
                 //closing
              else if (curr==2){
                  if(pre==1)
                  {
                      cursor.setCharFormat( format );
                      cursor.insertText( word );
                      l=l-1;
                  }
                  if(pre==2)
                  {
                      l=l-1;
                      cursor.insertText("\n");
                      for(int i=0;i<l;i++){cursor.insertText("  ");}
                      cursor.setCharFormat( format );
                      cursor.insertText( word );
                  }
                  else
                  {

                      cursor.setCharFormat( format );
                      cursor.insertText( word );
                      l=l-1;
                  }
              }
                  //value
              else if(curr==3){

                  format.setForeground( QBrush( QColor(Qt::black) ) );
                  cursor.setCharFormat(format);
                  cursor.insertText( word );
               }
          //lone tag
              else if(curr==4){

                  cursor.insertText("\n");
                  for(int i=0;i<l;i++){cursor.insertText("  ");}
                  format.setForeground( QBrush( QColor(Qt::darkMagenta) ) );
                  cursor.setCharFormat( format );
                  cursor.insertText( word );
              }
          //comment or prolog
              else {
                  cursor.insertText("\n");
                  format.setForeground( QBrush( QColor(Qt::darkGreen) ) );
                  cursor.setCharFormat( format );
                  cursor.insertText( word );
              }
          wordpre=word;
             }
             cursor.~QTextCursor();
             format.setForeground( QBrush( QColor(Qt::black) ) );
             ui->output_text->setCurrentCharFormat(format);
             }

         else{
                format.setForeground( QBrush( QColor(Qt::black) ) );
                 while (!tagsfile.atEnd())
                           {

                     word = tagsfile.readLine().trimmed();
                     if(word.isEmpty()){continue;}
                     curr=classification(word);
                     pre=classification(wordpre);

                           //opening
                               if(curr==1){
                                   if(pre==0)
                                   {
                                   str<<word;
                                   l=l+1;
                                   }
                                   else if(pre==1)
                                   {
                                   str<<"\n";
                                   for(int i=0;i<l;i++){str<<"  ";}
                                   str<<word;
                                   l=l+1;
                                   }
                                   else
                                   {
                                   str<<"\n";
                                   for(int i=0;i<l;i++){str<<"  ";}
                                   str<<word;
                                   l=l+1;
                                   }
                               }
                           //closing
                               else if (curr==2){
                                   if(pre==1)
                                   {
                                       str<<word;
                                       l=l-1;
                                   }
                                   if(pre==2)
                                   {
                                       l=l-1;
                                       str<<"\n";
                                       for(int i=0;i<l;i++){str<<"  ";}
                                       str<<word;
                                   }
                                   else
                                   {
                                       str<<word;
                                       l=l-1;
                                   }
                               }
                           //value
                               else if(curr==3){
                                   str<<word;
                                }
                           //lone tag
                               else if(curr==4){
                                   str<<"\n";
                                   for(int i=0;i<l;i++){str<<"  ";}
                                   str<<word;
                               }
                           //comment or prolog
                               else {
                                   str<<"\n";
                                   str<<word;
                               }
                           wordpre=word;
                           }
                 tempxml.close();
                    tempxml.open(QIODevice::ReadWrite |QIODevice::Text);
                    QTextStream strq(&tempxml);
                    ui->output_text->clear();
                    QString t=strq.readAll();
                    ui->output_text->setPlainText(t);

         }
             tagsfile.close();
             tempxml.close();
}
    

void MainWindow::on_Reset_button_clicked()
{
        qApp->quit();
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

void MainWindow::on_Exit_Button_clicked()
{
    qApp->quit();
}
///////////////////shaymaa////////////////////////////////////////////////////////////////////////////////////////
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
void toJson (Node* root)
{

    root->data = "\"" + root->data + "\"";
    Quote(root);
    Brackets(root);
    //regulateHyberTags(root);
    print(root);
    json[json.length()-1] = '}';
    return;
}
int classification(QString w) {//1:opening 2:closing 3:value  4:lone tag  5:comment  6:prolog
   if(w.isEmpty()){return 0;}
    if((w[0]=='<')&&w[w.length()-1]=='>')
    {
       if(w[1]=='/'){return 2;}
       if(w[w.length()-2]=='/'){return 4;}
       if(w[1]=='?'){return 6;}
       if(w[1]=='!'){return 5;}
       return 1;
    }
    return 3;
}
std::vector<unsigned int> errors;
std::vector<std::string> tagsErrors;
std::vector<int> Cases;

void findError() {
    errors.resize(0);
    tagsErrors.resize(0);
    Cases.resize(0);

    for (unsigned int k = 0;k < lines.size();k++) {

        if (lines[k].empty()) {
            tagsErrors.push_back(lines[k]);
            continue;
        }
    if (classification(QString::fromStdString(lines[k])) == 4 || classification(QString::fromStdString(lines[k])) == 5 || classification(QString::fromStdString(lines[k])) == 6) { continue; }

        int counter = std::count(lines[k].begin(), lines[k].end(), '<');
        int p1 = lines[k].find('<');
        int p2 = lines[k].find('>');
    if (counter == 0) {
            tagsErrors.push_back("~" + lines[k]);
            continue;
        }

        for (int l = 0;l < counter;l++) {
            if (l == 0) {
                std::string s = lines[k].substr(p1 + 1, p2 - p1 - 1);
                s = s.substr(0, s.find(' '));
                tagsErrors.push_back(s);
            }
            else {
                std::string s = lines[k].substr(p1 + 1, p2 - p1 - 1);
                s = s.substr(0, s.find(' '));
                tagsErrors.back() = tagsErrors.back() + "-" + s;
            }

            int p3 = lines[k].find('<', p1 + 1);
            int p4 = lines[k].find('>', p2 + 1);
            p1 = p3;
            p2 = p4;
        }
    }

    std::vector<std::string> temp;
    std::vector<int> in;
    for (unsigned int j = 1;j < tagsErrors.size() + 1;j++) {

        if (tagsErrors[j - 1].empty()) {
            errors.push_back(j);
            continue;
        }
        else if (tagsErrors[j - 1][0] == '~') {
            continue;
        }
if (tagsErrors[j - 1].find('/') == std::string::npos) {
            temp.push_back(tagsErrors[j - 1]);
            in.push_back(j);
        }
      else {
            std::stringstream check1(tagsErrors[j - 1]);

            std::string intermediate;
             while (getline(check1, intermediate, '-'))
            {
                if (intermediate.find('/') == std::string::npos) {
                    temp.push_back(intermediate);
                    in.push_back(j);
                }
  else {
                    std::string s = intermediate.substr(1, intermediate.length() - 1);
                    if (temp.back() == s) {
                        temp.pop_back();
                        in.pop_back();
                    }
                    else if (temp[temp.size() - 2] == s) {
                        errors.push_back(in.back());
                        Cases.push_back(1);
                        temp.pop_back();
                        in.pop_back();
                        j--;
                    }
      else {
                        errors.push_back(j);
                        Cases.push_back(2);
                        temp.pop_back();
                        in.pop_back();
                    }
                }
            }
        }
    }
 sort(errors.begin(), errors.end());
    return;
}
