#ifndef NODE_H
#define NODE_H
#include <string>
#include <QString>

class Node
{
public:

    enum class _nodeType{
        FOLDER,
        FILE
    };

private:
    std::string _name;
    _nodeType _type;
    Node* _parentNode;
    unsigned int numberOfChildNodes = 0;
    unsigned int capasityNumberOfChildren = 30;
    Node** childNodes = new Node*[30];

    void resizeArrayOfChildren();
public:
    Node(std::string name, _nodeType nodeType, Node* parentNode = nullptr);

    std::string getPath();
    _nodeType getType();
    std::string getName();
    unsigned int getNumberOfChildren();
    Node* getChild(unsigned int i);
    Node** getChildren();
    void addChild(Node* childNode);
    static QString toQString(_nodeType type);

    ~Node();
};

#endif // NODE_H
