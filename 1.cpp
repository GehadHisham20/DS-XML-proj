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
