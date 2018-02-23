#ifndef SCHEMA_H
#define SCHEMA_H

#include <string>
#include <vector>

#include "types.h"

class Node {
private:
    std::string _name;
    Type::type _type;
    Encoding::type _encoding;
    bool _isNullable = false;
public:
    Node(std::string name, Type::type type)
        :_name(name),_type(type),_encoding(Encoding::PLAIN),_isNullable(false) {}
    Node(std::string name, Type::type type, Encoding::type encoding)
        :_name(name),_type(type),_encoding(encoding),_isNullable(false) {}
    Node(std::string name, Type::type type, bool isNullable)
        :_name(name),_type(type),_encoding(Encoding::PLAIN),_isNullable(isNullable) {}
    Node(std::string name, Type::type type, Encoding::type encoding, bool isNullable)
        :_name(name),_type(type),_encoding(encoding),_isNullable(isNullable) {}
    Type::type getType()
    {
        return this->_type;
    }
    Encoding::type getEncoding()
    {
        return this->_encoding;
    }
    bool isNullable()
    {
        return this->_isNullable;
    }
    bool operator ==(const Node& node)
    {
        return this->_name == node._name;
    }
    bool operator ==(const std::string name)
    {
        return this->_name == name;
    }
    friend std::ostream& operator <<(std::ostream& out, const Node& node)
    {
        out << node._name << " " << node._type << " " << node._encoding << " " << node._isNullable;

        return out;
    }
};

class Schema {
private:
    std::vector<Node> _nodes;
public:
    Schema():_nodes(std::vector<Node>()) {}
    uint64_t size()
    {
        return this->_nodes.size();
    }
    Schema& push(Node node)
    {
        for(uint64_t i = 0; i < _nodes.size(); i++)
        {
            Node value = _nodes.at(i);
            if (value == node) {
                throw;
            }
        }

        _nodes.push_back(node);

        return *this;
    }
    Node peek(uint64_t pos)
    {
        return _nodes.at(pos);
    }
    uint64_t position(std::string name)
    {
        for(uint64_t i = 0; i < _nodes.size(); i++)
        {
            Node node = _nodes.at(i);
            if (node == name) {
                return i;
            }
        }

        throw;
    }
    friend std::ostream& operator <<(std::ostream& out, const Schema& schema)
    {
        for(uint64_t i = 0; i < schema._nodes.size(); i++) {
            Node node = schema._nodes.at(i);
            out << node << std::endl;
        }

        return out;
    }
};

#endif // SCHEMA_H
