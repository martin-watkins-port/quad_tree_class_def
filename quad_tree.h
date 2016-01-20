// ===================================================================
//
// We provide the Point class and the implementation of several
// QuadTree member functions for printing.
//
// IMPORTANT: You should modify this file to add all of the necessary
// functionality for the QuadTree class and its helper classes: Node,
// DepthIterator, and BreadthIterator.
//
// ===================================================================
//
//
//  Work done by Martin Watkins is indicated in the comments 
//
//
//


#ifndef quad_tree_h_
#define quad_tree_h_

#include <iostream>
#include <vector>
#include <cassert>


// ==============================================================
// ==============================================================
// A tiny templated class to store 2D coordinates.  This class works
// with number_type = int, float, double, unsigned char (a 1 byte=8
// bit integer), short (a 2 byte=16 bit integer).

template <class number_type>
class Point {
public:
  Point(const number_type& x_, const number_type& y_) : x(x_),y(y_) {}
  // REPRESENTATION
  number_type x;
  number_type y;
};

// a helper function to print Points to STL output stream
template <class number_type>
inline std::ostream& operator<<(std::ostream &ostr, const Point<number_type> &pt) {
  ostr << "(" << pt.x << "," << pt.y << ")";
  return ostr;
}

//NODE CLASS -- MW
template <class number_type,class label_type>
class Node {
public:
  Node() : parent(NULL), pt(Point<number_type>(number_type(),number_type())), label(label_type()) {for(int i=0;i<4;i++) children[i] = NULL;}
  Node(const Point<number_type>& init_pt, const label_type& lbl) : pt(init_pt), label(lbl), parent(NULL) {for(int i=0;i<4;i++) children[i] = NULL;}
  //represented as a point, with a label, and pointers to children and its parent
  Point<number_type> pt;
  label_type label;
  Node<number_type,label_type>* children[4];
  Node<number_type,label_type>* parent;
};

//QUADTREE PROTOTYPE -- MW
template <class number_type,class label_type> class QuadTree;

//DF ITERATOR CLASS -- MW
template <class number_type,class label_type>
class DepthIterator {
public:
//CONSTRUCTORS -- MW
  DepthIterator() : ptr_(NULL) {}
  DepthIterator(Node<number_type, label_type>* p) : ptr_(p) {}

//OPERATOR * ---- very straightforward  -- MW
  const Point<number_type>& operator*() const { return ptr_->pt; }

//GETTER DONE'S -- MW
  const label_type& getLabel() const
  {
      return ptr_->label;
  }

  const int getDepth() const
  {
      //simply iterates and keeps track of how many steps to the root
      int depth = 0;
      Node<number_type, label_type>* temp = ptr_;
      while(temp->parent != NULL)
      {
          temp = temp->parent;
          depth++;
      }
      return depth;
  }
  
//

//COMPARISON OPERATORS -- MW
  bool operator== (const DepthIterator& rgt) { return (ptr_ == rgt.ptr_); }
  bool operator!= (const DepthIterator& rgt) { return (ptr_ != rgt.ptr_); }

//HELPER FUNCTION TO FIND OUT WHICH CHILD ptr_ POINTS TO -- MW
  int getChildNumber(Node<number_type, label_type>* p)
  {
      if(p->label == p->parent->children[0]->label)
        return 0;
      else if(p->label == p->parent->children[1]->label)
        return 1;
      else if(p->label == p->parent->children[2]->label)
        return 2;
      else if(p->label == p->parent->children[3]->label)
        return 3;
      else
        return -1;
  }

//PRE-INCREMENT OPERATOR -- MW
  DepthIterator<number_type, label_type> & operator++() {
    if(ptr_->children[0])
    {
        ptr_ = ptr_->children[0];
    }else if(ptr_->children[1])
    {
        ptr_ = ptr_->children[1];
    }else if(ptr_->children[2])
    {
        ptr_ = ptr_->children[2];
    }else if(ptr_->children[3])
    {
        ptr_ = ptr_->children[3];
    }else
    {
        Node<number_type, label_type>* parental = ptr_->parent;
        int i = getChildNumber(ptr_);
        if(i == -1)
        {
            ptr_ = NULL;
        }
        if(parental == NULL)
        {
            ptr_ = NULL;
        }else
        {
            if(i+1 == 4)
            {
                int j = getChildNumber(parental);
                ptr_ = parental->parent->children[j+1];
            }else
            {
                ptr_ = parental->children[i+1];
            }
        }
    }
    return *this;
  }

//POST INCREMENT OPERATOR -- MW
  DepthIterator<number_type, label_type> operator++(int) {
    DepthIterator<number_type, label_type> temp(*this);
    ++(*this);
    return temp;
  }

private:
//REPRESENTED AS A POINTER TO A NODE -- MW
  Node<number_type, label_type>* ptr_;
};

//BF ITERATOR CLASS -- MW
template <class number_type,class label_type>
class BreadthIterator {
public:
//CONSTRUCTORS -- MW
  BreadthIterator() : ptr_(NULL) {}
//SPECIAL CONSTRUCTOR THAT SIMULATES A BREADTH-FIRST TRAVERSAL, RECORDING THE STEPS IN A VECTOR -- MW
  BreadthIterator(Node<number_type, label_type>* p) : ptr_(p)
  {
      Node<number_type, label_type>* root = ptr_;
      findRoot(root);
      breadth_sim(root);
  }

//OPERATOR *  -- MW
  const Point<number_type>& operator*() const { return ptr_->pt; }

//GETTER DONE'S -- MW
  const label_type& getLabel() const
  {
      return ptr_->label;
  }

  int getDepth() const
  {
      int depth = 0;
      Node<number_type, label_type>* temp = ptr_;
      while(temp->parent != NULL)
      {
          temp = temp->parent;
          depth++;
      }
      return depth;
  }
// 

//COMPARISON OPERATORS -- MW
  bool operator== (const BreadthIterator& rgt) { return (ptr_ == rgt.ptr_); }
  bool operator!= (const BreadthIterator& rgt) { return (ptr_ != rgt.ptr_); }

//SAME HELPER FUNCTION AS DF_ITER -- MW
  int getChildNumber(Node<number_type, label_type>* p)
  {
      if(p->label == p->parent->children[0]->label)
        return 0;
      else if(p->label == p->parent->children[1]->label)
        return 1;
      else if(p->label == p->parent->children[2]->label)
        return 2;
      else if(p->label == p->parent->children[3]->label)
        return 3;
      else
        return -1;
  }

//HELPER FUNCTION THAT FINDS THE ROOT OF A NODE -- MW
  void findRoot(Node<number_type, label_type>* root)
  {
      if(root == NULL) return;
      while(root->parent != NULL)
      {
          root = root->parent;
      }
  }

//THE ACTUAL BF SIMULATION --- BASED ON THE IN-CLASS TRAVERSAL -- MW
  void breadth_sim(Node<number_type, label_type>* root)
  {
      std::vector<Node<number_type, label_type>* > current;
      std::vector<Node<number_type, label_type>* > next;

      //handle an empty tree
      if (root != NULL) {
          current.push_back(root);
      }
      //loop over all levels of the tree
      while (current.size() > 0) {
          //loop over all elements on this level
          for (int x = 0; x < current.size(); x++) {
            //PUSHING BACK TO THE "ORDER"
            order.push_back(current[x]);
            //MODIFIED PORTION --- ACCOUNTS FOR FOUR CHILDREN INSTEAD OF TWO
            if (current[x]->children[0] != NULL)
                next.push_back(current[x]->children[0]);
            if (current[x]->children[1] != NULL)
                next.push_back(current[x]->children[1]);
            if (current[x]->children[2] != NULL)
                next.push_back(current[x]->children[2]);
            if (current[x]->children[3] != NULL)
                next.push_back(current[x]->children[3]);
          }
          //switch to the next level, empty next vector to receive following level
          current = next;
          next.clear();
      }
  }

//PRE-INCREMENT OPERATOR -- MW
  BreadthIterator<number_type, label_type> & operator++() {
    //IF FIRST NODE, BEGIN WITH FIRST CHILD
    if(ptr_->parent == NULL) ptr_ = ptr_->children[0];
    else
    {
        //ELSE LOOP THROUGH THE PRE-RECORDED ORDER
        for(int i=0;i<order.size();i++)
        {
            //FIND PTR_
            if(order[i]->label == ptr_->label)
            {
                //IF PTR_ IS THE LAST ELEMENT, RETURN EFFECTIVELY .END()
                if(i == (order.size()-1)) ptr_ = NULL;
                else
                {
                    //ELSE SET PTR_ TO THE NEXT STEP IN "ORDER" AND GET THE HECK OUT OF THE LOOP
                    ptr_ = order[i+1];
                    break;
                }
            }
        }
    }
    return *this;
  }

//POST-INCREMENT OPERATOR -- MW
  BreadthIterator<number_type, label_type> operator++(int) {  // post-increment, itr++
    BreadthIterator<number_type, label_type> temp(*this);
    ++(*this);
    return temp;
  }

private:
//REPRESENTED AS A POINTER TO A NODE, AND A VECTOR OF "STEPS" IN THE GRAND SCHEME OF THE TREE -- MW
  Node<number_type, label_type>* ptr_;
  std::vector< Node<number_type, label_type>* > order;
};

//QuadTree CLASS -- MW
template <class number_type,class label_type>
class QuadTree {
public:
//CONSTRUCTOR -- MW
  QuadTree() : root_(NULL), size_(0) {}
//COPY CONSTRUCTOR -- MW
  QuadTree(const QuadTree<number_type, label_type>& old) : size_(old.size_) {
    root_ = this->copy_tree(old.root_,NULL); }
//ASSIGNMENT OPERATOR -- MW
  QuadTree& operator=(const QuadTree<number_type, label_type>& old) {
    if (&old != this) {
      this->destroy_tree(root_);
      root_ = this->copy_tree(old.root_,NULL);
      size_ = old.size_;
    }
    return *this;
  }
//DESTROYER OF WORLDS -- MW
  ~QuadTree() {
    this->destroy_tree(root_);
    root_ = NULL;
  }

//COUPLE OF TYPEDEFS -- MW
  typedef DepthIterator<number_type, label_type> iterator;
  friend class DepthIterator<number_type, label_type>;
  typedef BreadthIterator<number_type, label_type> bf_iterator;
  friend class BreadthIterator<number_type, label_type>;

//ACCESSORS -- MW
  unsigned int size() const { return size_; }
  bool operator==(const QuadTree<number_type, label_type>& old) const { return (old.root_ == this->root_); }

//A HELPER FUNCTION, RECURSING THROUGH EACH BRACH TO FIND THE LONGEST, AND RETURNING THAT HEIGHT -- MW
  unsigned int recurse_height(Node<number_type, label_type>* p) const
  {
    unsigned int height = 0;
    if (p) {
        if (p->children[0] || p->children[1] || p->children[2] || p->children[3]) {
            unsigned int height0 = 0, height1 = 0, height2 = 0, height3 = 0;
            if (p->children[0]) height0 = recurse_height(p->children[0]);
            if (p->children[1]) height1 = recurse_height(p->children[1]);
            if (p->children[2]) height2 = recurse_height(p->children[2]);
            if (p->children[3]) height3 = recurse_height(p->children[3]);

            if (height0 > height1 && height0 > height2 && height0 > height3)
                height = height0 + 1;
            else if (height1 > height0 && height1 > height2 && height1 > height3)
                height = height1 + 1;
            else if (height2 > height0 && height2 > height1 && height0 > height3)
                height = height2 + 1;
            else
                height = height3 + 1;
        }
    }
    return height;
  }

//HEIGHT DRIVER FUNCTION -- MW
  unsigned int height() const
  {
      unsigned int height = -1;
      if(size_ > 0)
      {
          height = recurse_height(root_);
      }
      return height;
  }

//FIND -- MW
  iterator find(const number_type& x,const number_type& y) { return find(x,y, root_); }
//INSERT -- MW
  std::pair< iterator, bool > insert(const Point<number_type>& newp, const label_type& lbl) { return insert(newp, lbl, root_, NULL); }

//PRINT FUNCTIONS -- MW
  void plot(int max_x, int max_y, bool draw_lines) const;
  void plot(Node<number_type,label_type> *p, std::vector<std::string> &board,
          int x_min, int x_max, int y_min, int y_max, bool draw_lines) const;
  void print_sideways() const;
  void print_sideways(Node<number_type,label_type>* p, const std::string &indent) const;


//ITERATORS AND BF_ITERATORS -- MW
  iterator begin() const {
    if (!root_) return iterator(NULL);
    return iterator(root_);
  }

  iterator end() const { return iterator(NULL); }

  bf_iterator bf_begin() const
  {
      if(!root_) return bf_iterator(NULL);
      return bf_iterator(root_);
  }

  bf_iterator bf_end() const { return bf_iterator(NULL); }

private:
//REPRESENTATION -- MW
  Node<number_type, label_type>* root_;
  unsigned int size_;

//PRIVATE HELPER FUNCTIONS
//==============================================================================================================================
//FUNCTION TO COPY THE QUADTREE RECURSIVELY --- INSPIRED BY DS_SET -- MW
  Node<number_type, label_type>*  copy_tree(Node<number_type, label_type>* old_root, Node<number_type, label_type>* the_parent) {
    if (old_root == NULL)
      return NULL;
    Node<number_type, label_type> *answer = new Node<number_type, label_type>();
    answer->label = old_root->label;
    answer->pt = old_root->pt;
    answer->children[0] = copy_tree(old_root->children[0],answer);
    answer->children[1] = copy_tree(old_root->children[1],answer);
    answer->children[2] = copy_tree(old_root->children[2],answer);
    answer->children[3] = copy_tree(old_root->children[3],answer);
    answer->parent = the_parent;
    return answer;
  }

//FUNCTION TO DESTROY THE QUADTREE RECURSIVELY --- INSPIRED BY DS_SET -- MW
  void destroy_tree(Node<number_type, label_type>* p) {
    if (!p) return;
    destroy_tree(p->children[0]);
    destroy_tree(p->children[1]);
    destroy_tree(p->children[2]);
    destroy_tree(p->children[3]);
    delete p;
  }

//FUNCTION TO FIND A NODE RECURSIVELY --- INSPIRED BY DS_SET -- MW
  iterator find(const number_type& x, const number_type& y, Node<number_type, label_type>* p) {
    if (!p) return end();
    if (p->pt.x > x && p->pt.y > y)
        return find(x, y, p->children[0]);
    else if (p->pt.x < x && p->pt.y > y)
        return find(x, y, p->children[1]);
    else if (p->pt.x > x && p->pt.y < y)
        return find(x, y, p->children[2]);
    else if (p->pt.x < x && p->pt.y < y)
        return find(x, y, p->children[3]);
    else
        return iterator(p);
  }

//FUNCTION TO INSERT A NODE IN THE PROPER PLACE RECURSIVELY --- INSPIRED BY DS_SET -- MW
  std::pair<iterator,bool> insert(const Point<number_type>& newp, const label_type& lbl, Node<number_type, label_type>*& p, Node<number_type, label_type>* the_parent) {
    if (!p) {
      p = new Node<number_type, label_type>(newp,lbl);
      p->parent = the_parent;
      this->size_++;
      return std::pair<iterator,bool>(iterator(p), true);
    }
    else if (p->pt.x > newp.x && p->pt.y > newp.y)
        return insert(newp, lbl, p->children[0], p);
    else if (p->pt.x < newp.x && p->pt.y > newp.y)
        return insert(newp, lbl, p->children[1], p);
    else if (p->pt.x > newp.x && p->pt.y < newp.y)
        return insert(newp, lbl, p->children[2], p);
    else if (p->pt.x < newp.x && p->pt.y < newp.y)
        return insert(newp, lbl, p->children[3], p);
    else
        return std::pair<iterator,bool>(iterator(p), false);
  }
};
// ==============================================================
// PROVIDED CODE : QUAD TREE MEMBER FUNCTIONS FOR PRINTING
// ==============================================================

// NOTE: this function only works for quad trees with non negative
// integer coordinates and char labels

// NOTE2: this function assumes that no two points have the same x
// coordinate or the same y coordinate.

// plot driver function
// takes in the maximum x and y coordinates for these data points
// the optional argument draw_lines defaults to true
template <class number_type,class label_type>
void QuadTree<number_type,label_type>::plot(int max_x, int max_y, bool draw_lines=true) const {
  // allocate blank space for the center of the board
  std::vector<std::string> board(max_y+1,std::string(max_x+1,' '));
  // edit the board to add the point labels and draw vertical and
  // horizontal subdivisions
  plot(root_,board,0,max_x,0,max_y,draw_lines);
  // print the top border of the plot
  std::cout << "+" << std::string(max_x+1,'-') << "+" << std::endl;
  for (int i = 0; i <= max_y; i++) {
    // print each row of the board between vertical border bars
    std::cout << "|" << board[i] << "|" << std::endl;
  }
  // print the top border of the plot
  std::cout << "+" << std::string(max_x+1,'-') << "+" << std::endl;
}

// actual recursive function for plotting
template <class number_type,class label_type>
void QuadTree<number_type,label_type>::plot(Node<number_type,label_type> *p, std::vector<std::string> &board,
          int x_min, int x_max, int y_min, int y_max, bool draw_lines) const {
  // base case, draw nothing if this node is NULL
  if (p == NULL) return;
  // check that the dimensions range of this node make sense
  assert (x_min >= 0 && x_min <= x_max);
  assert (y_min >= 0 && y_min <= y_max);
  assert (board.size() >= y_max);
  assert (board[0].size() >= x_max);
  // verify that the point stored at this node fits on the board
  assert (p->pt.y >= 0 && p->pt.y < board.size());
  assert (p->pt.x >= 0 && p->pt.x < board[0].size());
  // draw the vertical and horizontal bars extending across the
  // range of this node
  if (draw_lines) {
    for (int x = x_min; x <= x_max; x++) {
      board[p->pt.y][x] = '-';
    }
    for (int y = y_min; y <= y_max; y++) {
      board[y][p->pt.x] = '|';
    }
  }
  // draw this label
  board[p->pt.y][p->pt.x] = p->label;
  // recurse on the 4 children
  plot(p->children[0],board,x_min ,p->pt.x-1,y_min ,p->pt.y-1,draw_lines);
  plot(p->children[1],board,p->pt.x+1,x_max ,y_min ,p->pt.y-1,draw_lines);
  plot(p->children[2],board,x_min ,p->pt.x-1,p->pt.y+1,y_max ,draw_lines);
  plot(p->children[3],board,p->pt.x+1,x_max ,p->pt.y+1,y_max ,draw_lines);
}


// ==============================================================

// prints all of the tree data with a pre-order (node first, then
// children) traversal of the tree structure

// driver function
template <class number_type,class label_type>
void QuadTree<number_type,label_type>::print_sideways() const { print_sideways(root_,""); }

// actual recursive function
template <class number_type,class label_type>
void QuadTree<number_type,label_type>::print_sideways(Node<number_type,label_type>* p, const std::string &indent) const {
  // base case
  if (p == NULL) return;
  // print out this node
  std::cout << indent << p->label << " (" << p->pt.x << "," << p->pt.y << ")" << std::endl;
  // recurse on each of the children trees
  // increasing the indentation
  print_sideways(p->children[0],indent+"  ");
  print_sideways(p->children[1],indent+"  ");
  print_sideways(p->children[2],indent+"  ");
  print_sideways(p->children[3],indent+"  ");
}

// ==============================================================
// ==============================================================


#endif
