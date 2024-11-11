#include "JumpList.h"
#include <stdexcept>
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
using std::cout;
using std::endl;

Node::Node(const string& s, Node* next, Node* jump, int gap) {
	// IMPLEMENT ME
    this->data_ = s;
    this->next_ = next;
    this->jump_ = jump;
    this->gap_ = gap;
}

Node::~Node() {
	// IMPLEMENT ME
    // delete this->next_;
    // delete this->jump_;
    delete this;
}


JumpList::JumpList() {
	// IMPLEMENT ME
    this->head_ = nullptr;
}

// DO NOT CHANGE
// You do not need to understand the code here to complete the assignment
JumpList::JumpList(int size, const string* arr) {

	const string s[] = {"a", "b", "blah", "c", "cat", "d", "etc", "ooo...", "x", "zzz"};
	const string* sp = (arr == nullptr) ? s : arr;

	Node** npp = new Node*[size];
	npp[size-1] = new Node(sp[size-1], nullptr, nullptr, 0);
	for(int i = size-2; i >= 0; i--)
		npp[i] = new Node(sp[i], npp[i+1], nullptr, 0);

	if (arr == nullptr) {

		if (size==1) npp[0]->gap_ = 1;
		else if (size==3) npp[0]->gap_ = 3;
 		else if (size==10) {
			npp[0]->jump_ = npp[5];
			npp[5]->jump_ = npp[8];
			npp[0]->gap_ = 5;
			npp[5]->gap_ = 3;
			npp[8]->gap_ = 2;
		}
		else throw std::invalid_argument("size must be 1, 3 or 10");

	}
	else {

		// jump node every MAX_GAP_SIZE
		int i;
		for(i=0; i < size-MAX_GAP_SIZE; i += MAX_GAP_SIZE) {
			npp[i]->jump_ = npp[i+MAX_GAP_SIZE];
			npp[i]->gap_ = MAX_GAP_SIZE;
		}
		npp[i]->gap_ = size % MAX_GAP_SIZE;
		if (npp[i]->gap_ == 0) npp[i]->gap_ = MAX_GAP_SIZE; // 0 need to become 5!

	}

	head_ = npp[0];
	delete [] npp; // note that the local array npp gets destroyed here but the nodes stay alive!

}

JumpList::~JumpList() {
	// IMPLEMENT ME
    // Node *tmp;
    // while(this->head_ != nullptr) {
    //     tmp = this->head_;
    //     this->head_ = this->head_->next_;
    //     delete tmp;
    // }
}


int JumpList::size() const {
    int size = 0;

    Node *count_node = this->head_;
    
    while (count_node != nullptr) {
        size++;
        count_node = count_node->next_;
    }
	return size; // dummy
}

// DONE FOR YOU, DO NOT CHANGE
bool JumpList::find(const string& s) const {

	if (head_ == nullptr) return false;

	// moving along the fast lane
	Node* tmp = head_;
	while(tmp->jump_ != nullptr && tmp->jump_->data_ < s)
		tmp = tmp->jump_;

	// tmp now points to the jump node at the start of the segment where s could be

	// now slow lane
	while(tmp != nullptr) {

		if (tmp->data_ == s) return true; // match
		else if (tmp->data_ > s) return false; // went past without finding s
		else tmp = tmp->next_;
	}

	return false; // end of list
}

string JumpList::get(int i) const {
	// IMPLEMENT ME
    if (i < 0 || i >= this->size()) {
        return "";
    }

    int position = 0;
    Node *temp = this->head_;
    if (i == 0) {
        return head_->data_;
    }
    while (position <= i || temp != nullptr) {
        if (position + temp->gap_ == i) {
            return temp->jump_->data_;
        }else if (position + temp->gap_ > i) {
                break;
            }
        position += temp->gap_;
        temp = temp->jump_;
    }
    while (position != i) {
        temp = temp->next_;
        position++;
    }

	return temp->data_; // dummy
}

string JumpList::print() const {
	// IMPLEMENT ME
    string jumplist = "";
    string jumpnodes = "";
    string jumpgaps = "";
    string result;
    Node *temp = this->head_;
    while (temp != nullptr) {
        jumplist += temp->data_ + " ";

        if(temp->gap_ > 0) {
            jumpnodes += temp->data_ + " ";
            jumpgaps += std::to_string(temp->gap_) + " ";
        }
        temp = temp->next_;
    }
    if (this->size() >= 1) {
    result = jumplist.substr(0, jumplist.length() - 1) + "\n" + jumpnodes.substr(0, jumpnodes.length() - 1) + "\n" + jumpgaps.substr(0, jumpgaps.length() - 1);
    } else {
        result = "\n\n";
    }

	return result;
}

string JumpList::prettyPrint() const {
	// IMPLEMENT ME
    string jumplist = "";
    string jumpnodes = "";
    string jumpgaps = "";
    string result = "";
    std::vector<string> jump_nodes;
    std::unordered_map<string, int> jumpMap;
    Node *temp = this->head_;
    while (temp != nullptr) {
        jumplist += temp->data_ + " --> ";
        if(temp->gap_ > 0) {
            jump_nodes.push_back(temp->data_);
            jumpMap.insert({temp->data_, temp->gap_});
            // jumpnodes += temp->data_ + " ";
            // jumpgaps += std::to_string(temp->gap_) + " ";
        } else {
            // jumpnodes += "--";
        }
        temp = temp->next_;
    }

    if (this->size() >= 1) {
        result = jumplist.substr(0, jumplist.length() - 5);
    }

    for (char c: result) {
        jumpnodes += "-";
    }

    int idx = 0;
    while(idx <= result.length() - 1) {
        if (result[idx] != '-' || result[idx] != '>' || result[idx] != ' ') {
            int start_parse_pos = idx;
            int end_parse_pos = idx;
            while (result[idx] != ' ')
                idx++;
            end_parse_pos = idx;
            // cout << "start_parse_pos: " << start_parse_pos << endl;
            // cout << "end_parse_pos: " << end_parse_pos << endl;
            
            string sub_str = result.substr(start_parse_pos, end_parse_pos - start_parse_pos);
            cout << "Substring: " << sub_str << endl;
            if (std::find(jump_nodes.begin(), jump_nodes.end(), sub_str) != jump_nodes.end()) {
                if (start_parse_pos != 0) {
                    if (sub_str.length() > 1) { // e.g. sub_str = aaaa
                        jumpnodes = jumpnodes.replace(start_parse_pos - 1, (end_parse_pos - start_parse_pos) + 1,"> " + sub_str + " ");
                    } else { // e.g sub_str = a
                        jumpnodes = jumpnodes.replace(start_parse_pos - 2, (end_parse_pos - start_parse_pos) + 1,"> " + sub_str + " ");
                    }
                } else{ // first jumpnode does not need any space padding
                    jumpnodes = jumpnodes.replace(start_parse_pos, end_parse_pos - start_parse_pos, sub_str);
                }
            }
            // continue;
        }
        // jumpnodes += "--";
        idx++;
    }

    // jumpnodes
    if(this->size() > 0) {
        int j = 0; // make the replacement variable a--------> becomes a -------> OR aa--------> becomes aa -------->
        while (jumpnodes[j] != '-') { //  || jumpnodes[j] != ' '
            j++;
        }
        jumpnodes = jumpnodes.replace(j, j, " ");
        int i = jumpnodes.length();
        while (true) {
            cout << "i: " << i << "; char: " << jumpnodes[i] << endl;
            if (jumpnodes[i] == ' ') {
                jumpnodes = jumpnodes.substr(0, i);
                break;
            }
            i--;
        }
    }

    for (char c: jumpnodes) jumpgaps += " ";

    idx = 0;
    // cout << "result -> " << result << endl;
    // cout << "jumpnodes-> " << jumpnodes << endl;
    cout << "BUILDING JUMPGAPS" << endl;
    string gaps = "";
    if(jumpnodes.length() > 0){
        while (idx <= jumpnodes.length() - 1) {
            cout << "idx at start of loop: " << idx << endl;
            if (jumpnodes[idx] != '-' || jumpnodes[idx] != '>' || jumpnodes[idx] != ' ') {
                int start_parse_pos = idx;
                int end_parse_pos = 0;

                try
                {
                    while (jumpnodes[idx] != ' ') {
                        if (!jumpnodes[idx + 1]) {
                            end_parse_pos = idx+1;
                            break;
                        }
                        idx++;
                    }

                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                }
        
                end_parse_pos = idx;
                
                string sub_str = jumpnodes.substr(start_parse_pos, end_parse_pos - start_parse_pos);
                cout << "Substring: " << sub_str << endl;
                if (std::find(jump_nodes.begin(), jump_nodes.end(), sub_str) != jump_nodes.end()) {
                    string gap = std::to_string(jumpMap[sub_str]);

                    if (sub_str.length()>1) {
                        for (int i = 1; i < sub_str.length(); i++) {
                            gap += " ";
                        }
                    }
                
                    if ((start_parse_pos + (end_parse_pos - start_parse_pos)) >= jumpnodes.length()) {
                        gaps = jumpgaps.replace(start_parse_pos, jumpnodes.length(), gap);
                    } else {
                        gaps = jumpgaps.replace(start_parse_pos, end_parse_pos - start_parse_pos, gap);
                    }
                }
                // continue;
            }
            idx++;
        }
    } else if(jumpnodes.length() == 1) { // only one jump node
        gaps += std::to_string(jumpMap[std::to_string(jumpnodes[0])]);
    }

    result = result + "\n" + jumpnodes + "\n" + gaps;

	return result;
}

bool JumpList::insert(const string& s) {

    if (this->find(s)) return false; // check if s already exists

    if (this->head_ == nullptr) { // empty JumpList inse
        this->head_ = new Node(s, nullptr, nullptr, 1);
        this->print();
        return true;
    }

    Node *last_seen_jump_node = nullptr;
    if (head_->gap_ > 0) {
        last_seen_jump_node = head_;
    }

    if (head_->data_ > s) {
        cout << "executing prepend insert for s = " << s << endl;
        Node *new_head = new Node(s, head_, head_->jump_, head_->gap_);
        head_->jump_ = nullptr;
        head_->gap_ = 0;
        head_ = new_head;

        head_->gap_++;
        if (head_->gap_ > MAX_GAP_SIZE) { // handle split for prepend ops

            int mid_gap = head_->gap_ / 2;
            Node *tail_jump_node = head_->jump_;
            Node *new_mid_jump_node = head_;
            for (int i = 0; i < mid_gap; i++) {
                new_mid_jump_node = new_mid_jump_node->next_;
            }
            head_->jump_ = new_mid_jump_node;
            head_->gap_ = mid_gap;
            new_mid_jump_node->jump_ = tail_jump_node;
            new_mid_jump_node->gap_ = mid_gap;

        }

        return true;
    }
    
    Node *temp = this->head_;
    while (temp->jump_ != nullptr && temp->jump_->data_ < s) { // ride the fast lane to the right segment
        temp = temp->jump_;
        last_seen_jump_node = temp;
    }
    while (temp->next_ != nullptr) {
        if (temp->next_->data_ > s) {
            Node *new_node = new Node(s, temp->next_, nullptr, 0);
            temp->next_ = new_node;

            last_seen_jump_node->gap_++; // increment gap size
            if (last_seen_jump_node->gap_ > MAX_GAP_SIZE) {
                int mid_gap = last_seen_jump_node->gap_ / 2;
                Node *tail_jump_node = last_seen_jump_node->jump_;
                Node *new_mid_jump_node = last_seen_jump_node;
                for (int i = 0; i < mid_gap; i++) {
                    new_mid_jump_node = new_mid_jump_node->next_;
                }
                last_seen_jump_node->jump_ = new_mid_jump_node;
                last_seen_jump_node->gap_ = mid_gap;
                new_mid_jump_node->jump_ = tail_jump_node;
                new_mid_jump_node->gap_ = mid_gap;
            }
            //
            return true;
        }
        temp = temp->next_;
    }

    temp->next_ = new Node(s, nullptr, nullptr, 0); // append to the end of the jumplist

    if(last_seen_jump_node != nullptr) { // handle possible split for append ops

        last_seen_jump_node->gap_++; // increment gap size
        if (last_seen_jump_node->gap_ > MAX_GAP_SIZE) { // actual split handling
            int mid_gap = last_seen_jump_node->gap_ / 2;
            Node *tail_jump_node = last_seen_jump_node->jump_;
            Node *new_mid_jump_node = last_seen_jump_node;
            for (int i = 0; i < mid_gap; i++) {
                new_mid_jump_node = new_mid_jump_node->next_;
            }
            last_seen_jump_node->jump_ = new_mid_jump_node;
            last_seen_jump_node->gap_ = mid_gap;
            new_mid_jump_node->jump_ = tail_jump_node;
            new_mid_jump_node->gap_ = mid_gap;
        }
    }
    cout << "append insert" << endl;
	return true;
}

bool JumpList::erase(const string& s) {
	// IMPLEMENT ME
    if (head_==nullptr) return false; // empty JumpList

    cout << "start of erasusre for s = " << s << endl;


    if(!this->find(s)) return false; // data not in jumpList



    // check the head
    if(head_->data_ == s) {
        cout << "Node is head node " << s << endl;
        if (head_->next_ == nullptr) { // if the head is the last element
            cout << "Single Node JumpList erasure" << endl;

            head_ = nullptr; // need to explicitly set head to nullptr to prevent this->size() from access garbage value.
            cout << "returning - last element deletion" << endl;
            return true;
        }

        
        head_->next_->gap_ = head_->gap_;
        // head_->next_->gap_--; // I need to reread the questions to understand why it has to be done this way
        head_ = head_->next_;
        // delete delete_node;
        head_->gap_ = (head_->gap_ - 1) < 1? 1 : head_->gap_ - 1; // decrement gap_
        if (head_ == head_->jump_) {
            head_->jump_ = nullptr; // avoid self references
        }
        cout << "returning - head_ node erasure" << endl; 
        return true;
    }

    // other cases within the JumpList
    int position = 0;

    Node *last_seen_jump_node = head_;
    Node *tmp = head_;
    Node *prev_node = nullptr;

    while(tmp->jump_ != nullptr && tmp->jump_->data_ < s) {

        tmp = tmp->jump_;
        last_seen_jump_node = tmp;
    }

    // 
    while(tmp->data_ != s) {
        prev_node = tmp;
        tmp = tmp->next_;
    }
    last_seen_jump_node->gap_--; // decrement gap

    if(tmp->gap_ > 0) { // handle erasure for jump nodes :: && tmp->jump_ != nullptr

        if (tmp->jump_ != nullptr){
            Node *new_jump_node = last_seen_jump_node; // will start count from last seen jump node
            if (last_seen_jump_node->gap_ + tmp->gap_ > MAX_GAP_SIZE) {
                cout << "Jump segment merge to occur" << endl;
                int total_gap = last_seen_jump_node->gap_ + tmp->gap_;
                int left_mid_gap = total_gap % 2 == 0 ? total_gap / 2 : (total_gap / 2) + 1;
                int right_mid_gap = total_gap / 2;


                for (int i = 0; i < left_mid_gap; i++) new_jump_node = new_jump_node->next_; // get to the new jump node

                new_jump_node->gap_ = right_mid_gap; new_jump_node->jump_ = tmp->jump_;
                last_seen_jump_node->gap_ = left_mid_gap; last_seen_jump_node->jump_ = new_jump_node; // complete merge

                prev_node->next_ = tmp->next_ != nullptr? tmp->next_: nullptr;
                // delete tmp;
                cout << "returning - jump with merge" << endl;
                return true;
            } else {
                cout << "Jump segment merge not needed for jump node" << endl;
                last_seen_jump_node->gap_ += tmp->gap_;
                last_seen_jump_node->jump_ = tmp->jump_;
                prev_node->next_ = tmp->next_ != nullptr? tmp->next_: nullptr;

                return true;
            }
        } else { // No jump value on node, but gap value present
                // "should be here where there is no jump but we have a gap"

            // last_seen_jump_node++;  This caused something interesting 😅
            last_seen_jump_node->gap_++; // re-apply increment, we aren't decrementing the last seen anymore

            if (tmp->next_ != nullptr) { // if we aren't at the end of the JumpList
                if ((last_seen_jump_node->gap_ + tmp->gap_) <= MAX_GAP_SIZE) { // if the cumulative gap is smaller than MAX SIZE

                    // last_seen_jump_node->gap_ += tmp->gap_;
                    last_seen_jump_node->gap_++;
                    prev_node->next_ = tmp->next_;
                } else  {
                        // ((last_seen_jump_node->gap_ + tmp->gap_) > MAX_GAP_SIZE)
                        prev_node->next_ = tmp->next_;
                        tmp->next_->jump_ = tmp->jump_;
                        tmp->next_->gap_ = tmp->gap_; // decrement the gap since we're moving to the right by one shift;
                        tmp->next_->gap_--;
                }

            } else {
                prev_node->next_ = nullptr;
            }

            cout << "returning - gap but no jump" << endl;
            return true;
        }
    }

    // erasure for normal nodes

    prev_node->next_ = tmp->next_;


    cout << "returning - normal erasure" << endl;
	return true;
}