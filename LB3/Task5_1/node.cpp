#include "node.h"

Node::Node(std::string name, _nodeType nodeType, Node* parentNode):
    _name(name), _type(nodeType), _parentNode(parentNode){}

std::string Node::getPath(){
    if(_parentNode == nullptr){
        return "/";
    }
    std::string path = _parentNode->getPath();
    if(this->_type == Node::_nodeType::FOLDER){
        path += "/";
    }
    return path;
}

Node::_nodeType Node::getType(){
    return _type;
}

Node::~Node(){
    for(unsigned int i = 0; i < numberOfChildNodes; i++){
        delete childNodes[i];
    }
}

void Node::resizeArrayOfChildren(){
    capasityNumberOfChildren *= 2;
    Node** ArrayOfChildren = new Node*[capasityNumberOfChildren];
    for(unsigned int i = 0; i < numberOfChildNodes; i++){
        ArrayOfChildren[i] = childNodes[i];
    }
    delete[] childNodes;
    childNodes = ArrayOfChildren;
}

void Node::addChild(Node* childNode){
    if(numberOfChildNodes >= capasityNumberOfChildren){
        resizeArrayOfChildren();
    }
    childNodes[numberOfChildNodes++] = childNode;
}

std::string Node::getName(){
    return _name;
}

unsigned int Node::getNumberOfChildren(){
    return numberOfChildNodes;
}

QString Node::toQString(_nodeType type){
    switch (type) {
    case _nodeType::FOLDER:
        return QString::fromStdString("FOLDER");
        break;

    case _nodeType::FILE:
        return QString::fromStdString("FILE");
        break;
    }
}

Node* Node::getChild(unsigned int i){
    return childNodes[i];
}

Node** Node::getChildren(){
    return childNodes;
}
