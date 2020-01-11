#pragma once

#include <iostream>
#include <memory>

class Node{
public:
	virtual void Output(std::ostream& os) const = 0;
	virtual ~Node() = default;
	friend std::ostream &operator<<(std::ostream& out, const Node& node);
};

class IntNode: public Node{
public:
	IntNode(int value): value_(value) {}
	void Output(std::ostream& os) const override;

private:
	const int value_;

};

class DoubleNode: public Node{
public:
	DoubleNode(double value): value_(value) {}
	void Output(std::ostream& os) const override;

private:
	const double value_;
};

class StringNode: public Node{
public:
	StringNode(const std::string& value): value_(value) {}
	void Output(std::ostream& os) const override;

private:
	const std::string value_;

};

class VectorNode: public Node{
public:
	VectorNode(const std::vector<std::shared_ptr<Node>>& vec): vec_(vec) {}
	VectorNode() = default;
	void Output(std::ostream& os) const override;
	void AddNode(const std::shared_ptr<Node>& node){
		vec_.push_back(node);
	}

private:
	std::vector<std::shared_ptr<Node>> vec_;

};

class PairNode: public Node{
public:
	PairNode(std::pair<std::string, std::shared_ptr<Node>> pair): pair_(pair){}
	void Output(std::ostream& os) const override;

private:
	const std::pair<std::string, std::shared_ptr<Node>> pair_;

};

class UnordMapNode: public Node{
public:
	void AddPair(const std::string& key, const std::shared_ptr<Node>& value){
		map_[key] = value;
	}
	void Output(std::ostream& os) const override;

private:
	std::unordered_map<std::string, std::shared_ptr<Node>> map_;

};
