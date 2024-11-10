#include <iostream>
using std::cout;
using std::endl;

int main() {
    cout << "before find loop" << endl;
    cout << "tmp->data: " << tmp->data_ << ", tmp->gap: " << tmp->gap_ << " tmp->jump: " << tmp->jump_->data_ << endl;
    while(tmp->jump_ != nullptr && tmp->jump_->data_ < s)
    tmp = tmp->jump_;
    cout << "after find loop" << endl;
}
