#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>
#include <unordered_map>

#include "../include/output_node.h"

using namespace std;

ostream &operator<<(ostream& out, const Node& node){
	node.Output(out);
	return out;
}

void IntNode::Output(ostream& out) const{
	out << value_;
}

void DoubleNode::Output(std::ostream& out) const{
	out << setprecision(6)<< value_;
}

void StringNode::Output(std::ostream& out) const{
	out << '"' << value_ << '"';
}

void VectorNode::Output(std::ostream& out) const{
	out << '[';
	for(size_t i = 0; i < vec_.size(); ++i){
		vec_[i]->Output(out);
		if(i != vec_.size() - 1){
			out << ',';
		}
	}
	out << ']';
}

void PairNode::Output(std::ostream& out) const {
	out << '"' << pair_.first << '"'<< ':';
	pair_.second->Output(out);
}

void UnordMapNode::Output(std::ostream& out) const{
	out << '{';
	size_t counter = map_.size() - 1;
	for(auto it = map_.begin(); it != map_.end(); ++it){
		PairNode pair(*it);
		pair.Output(out);
		if(counter > 0){
			out << ',';
			counter -= 1;
		}
	}
	out << '}';
}
