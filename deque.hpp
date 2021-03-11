#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP
#define MAXBLOCK 200
#define MERGESIZE 60
#define SPLITNUM 190

#include "exceptions.hpp"

#include <cstddef>
int cnt=0;
namespace sjtu {

    template<class T>
    class deque {
        friend class iterator;
        friend  class const_iterator;
    private:
        class block {
        public:
            T **element;
            int currentsize;
            block *pre, *nxt;

            block() {
                pre = NULL, nxt = NULL;
                currentsize = 0;
                element = new T *[MAXBLOCK + 10];
            }

            block(const block &other) {
                pre = other.pre, nxt =  other.nxt;
                currentsize = other.currentsize;
                element = new T *[MAXBLOCK + 10];
                for (int i = 0; i < other.currentsize; ++i) {
                  //  cout<<++cnt<<endl;
                    //cout<<"*"<<endl;
                    element[i] = new T(*other.element[i]);
                }
            }

            ~block() {
                for (int i = 0; i < currentsize; ++i) {
                    delete element[i];
                }
                delete[] element;
            };

            void add(T key, int location) {//插入后变成第location位
               // cout<<key<<"&&&&&&&&&"<<location<<"***"<<currentsize<<endl;
                //key=1000000;
                for (int i = currentsize; i > location ; --i) {
                    element[i] = element[i - 1];
                }
                element[location] = new T(key);
              //  cout<<location<<""  ""<<endl;
                currentsize++;
            }

            void erase(int location) {//删除第location位
                for (int i = location - 1; i < currentsize - 1; ++i) {
                    element[i] = element[i + 1];
                }
                currentsize--;

            }

            block &operator=(const block &other) {
                if (this == &other) return *this;
                for (int i = 0; i < currentsize; ++i) {
                    delete element[i];
                }
                delete[] element;
                pre = nullptr, nxt = nullptr;
                currentsize = other.currentsize;
                element = new T *[MAXBLOCK + 10];
                for (int i = 0; i < currentsize; ++i) {
                    element[i] = new T(*other.element[i]);
                }
            }

            T &at(const size_t &pos) { return element[pos]; }

            const T &at(const size_t &pos) const { return element[pos]; }

            T &operator[](const size_t &pos) { return element[pos]; }

            const T &operator[](const size_t &pos) const { return element[pos]; }


        };

    public:
        void merge(block *temp1, block *temp2) {
            for (int i = temp1->currentsize; i < temp1->currentsize + temp2->currentsize; ++i) {
                temp1->element[i] = temp2->element[i - temp1->currentsize];
            }
            temp1->nxt->nxt = temp2->nxt;
            temp2->nxt->pre = temp1;
            for (int i = 0; i < temp2->currentsize; ++i) {
                delete temp2->element[i];
            }
            delete[] temp2->element;
            temp1->currentsize += temp2->currentsize;
        }//last to do
        void split(block *temp) {
            int num = temp->currentsize;
            block *nxttemp = new block;
            nxttemp->nxt = temp->nxt;
            nxttemp->pre = temp;
            temp->nxt = nxttemp;

            //if (temp->nxt==tail){  cout<<"%%%%"<<endl;}
            nxttemp->nxt->pre=nxttemp;

            for (int i = 0; i < temp->currentsize / 2; ++i) {
                nxttemp->element[i] = temp->element[i + temp->currentsize - temp->currentsize / 2];
            }nxttemp->currentsize = temp->currentsize / 2;
            temp->currentsize = temp->currentsize - temp->currentsize / 2;
        }

    public:  block *head, *tail;
        int num;

    public:
        class const_iterator;

        class iterator {
            friend class const_iterator;

            friend class deque;

        public:
            deque *headdequeu;
            block *headblock;
            int pos;
            /**
             * TODO add data members
             *   just add whatever you want.
             */
        public:
            iterator() {
                headblock = nullptr;
                headdequeu = nullptr;
                int pos = 0;//0 base
            }

            iterator(const iterator &other) {
                headdequeu = other.headdequeu;
                headblock = other.headblock;
                pos = other.pos;
            }

            iterator &operator=(const iterator &other) {
               // cout<<other.headdequeu->num<<"*******999999*"<<endl;
                if (this == &other) {
                 //   cout<<"  wrong in  operator="<<endl;
                    return *this; }
//memory leak may exist
               // maybeneed clear something//todo
               // cout<<other.headdequeu->num<<"*******999999*"<<endl;
              //  headdequeu= nullptr,headblock= nullptr;
                //cout<<other.headdequeu->num<<"*******999999*"<<endl;
                //cout<<other.headdequeu->num<<"&&&"<<endl;
               // cout<<other.headblock->currentsize<<"&&&"<<endl;
                //cout<<other.headdequeu->num<<"*******999999*"<<endl;
             // other.headdequeu->debug();
               // if (other.headdequeu==NULL) cout<<"^^^"<<endl;
                headdequeu =other.headdequeu;
                headblock = other.headblock;
                pos = other.pos;
               // if (headdequeu!=other.headdequeu){cout<<"^^^^^&&&&&"<<endl;}
                return *this;
            }

            /**
             * return a new iterator which pointer n-next elements
             *   if there are not enough elements, iterator becomes invalid
             * as well as operator-
             */
            iterator operator+(const int &n) const {
               // cout<<n<<"&&&"<<endl;
                iterator temp(*this);
                int nownum = headblock->currentsize;
               // cout<<nownum<<"   "<<temp.pos<<"  "
               // cout<<pos<<"&&&"<<endl;
               // if (headblock==)
               // if (headblock==)
              // cout<<temp.headdequeu->num<<"^^^^^"<<endl;
               // cout<<temp.headblock->currentsize<<"  num in iter+"<<endl;
                //cout<<nownum<<"  num in + iter "<<endl;
                int addnum = n;
                block *p = headblock;
               // cout<<this->headblock->nxt->currentsize<<"********"<<endl;
                addnum -= (nownum - pos);//todo with edge
                //cout<<nownum<<"   "<<temp.pos<<"  "<<addnum<<endl;
                while (addnum >= 0 && p->nxt != nullptr) {
                    p = p->nxt;
                    addnum -= p->currentsize;
                }
                if (p == headblock) {
                    //cout<<"***"<<endl;
                    temp.pos += n;
                //cout<<temp.pos<<"***"<<endl;
                }
                else if (p == headdequeu->tail) {
                    temp.pos=0;
                    temp.headblock=headdequeu->tail;
                    //throw invalid_iterator();
                } else {
                    addnum += p->currentsize;
                    temp.headblock = p;
                    temp.pos = addnum;
                }//cout<<++cnt<<endl;
                //delete p;
              //  cout<<temp.headdequeu->num<<"%%%%&&&$$$"<<endl;
                return temp;
                //TODO
            }

            iterator operator-(const int &n) const {
                iterator temp(this);
                int nownum = headblock->currentsize;
                int addnum = n;
                if (pos - n >= 0) {
                    temp.pos -= n;

                } else {
                    block *p = headblock;
                    int temppos = temp.pos;
                    int frontnum = n;
                    frontnum -= (temppos);//todo
                    while (frontnum > 0 && p != headdequeu->head) {
                        p = p->pre;
                        frontnum -= p->currentsize;
                    }
                    if (p == headdequeu->head) {
                        throw invalid_iterator();
                    } else {
                        frontnum += p->currentsize;
                        temp.pos = p->currentsize - frontnum;
                    }
                }
                return temp;
                //TODO
            }

            // return th distance between two iterator,
            // if these two iterators points to different vectors, throw invaild_iterator.
            int operator-(const iterator &rhs) const {//todo is draft
                if (headdequeu != rhs.headdequeu) {
                    throw invalid_iterator();
                }
                if (headblock == rhs.headblock) {
                    return pos - rhs.pos;
                } else {
                    int dis = 0;
                    block *p = rhs.headblock->nxt;
                    dis = rhs.headblock->currentsize - rhs.pos;
                    while (p != nullptr) {

                      // cout<<p<<"   "<<headblock<<endl;
                       //cout<<p->currentsize<<"   "<<headblock->currentsize<<"^^^^"<<endl;
                        if (p == headblock) {
                            //cout<<"****"<<endl;
                            dis += pos - rhs.pos;
                            //cout<<dis<<"&&&"<<endl;
                            break;//todo

                        }
                        dis += p->currentsize;
                        p = p->nxt;
                    }
                    if (p->nxt == nullptr) {
                        p = rhs.headblock;
                        dis = p->currentsize - rhs.pos;
                        while (p != nullptr) {
                            p = p->nxt;
                            if (p == headblock) {
                                dis += pos - rhs.pos;
                                break;
                            }
                            dis += p->currentsize;
                        }
                    } else return dis;
                    if (p == nullptr) { throw invalid_iterator(); }
                    else {
                        return -(dis);
                    }


                }

                //TODO
            }

            iterator &operator+=(const int &n) {
                *this=*this+n;
                return *this;
                //TODO
            }

            iterator &operator-=(const int &n) {
                *this=*this-n;
                return *this;
            }

            /**
             * TODO iter++
             */
            iterator operator++(int) {//modify it!!!!
                iterator temp(*this);
                *this = *this + 1;
                return temp;

            }

            /**
             * TODO ++iter
             */
            iterator &operator++() {
                //cout<< this->headdequeu->num<<"HHHHHH"<<endl;

                *this=*this+1;
               // cout<<this->headdequeu->num<<"&8888888888888888&&&"<<endl;
                return *this;
                //todo
            }

            /**
             * TODO iter--
             */
            iterator operator--(int) {
                iterator temp(*this);
                *this = *this - 1;
                return temp;
            }

            /**
             * TODO --iter
             */
            iterator &operator--() {
                *this=*this-1;
                return *this;
            }

            /**
             * TODO *it
             * 		throw if iterator is invalid
             */
            T &operator*() const {
                return *(headblock->element[pos]);

            }

            /**
             * TODO it->field
             * 		throw if iterator is invalid
             */
            T *operator->() const noexcept {
                return *(headblock->element[pos]);
            }

            /**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
            bool operator==(const iterator &rhs) const {
                return  (headdequeu==rhs.headdequeu&&headblock==rhs.headblock&&pos==rhs.pos);

            }

            bool operator==(const const_iterator &rhs) const {
                return  (headdequeu==rhs.headdequeu&&headblock==rhs.headblock&&pos==rhs.pos);
            }

            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                return  !(headdequeu==rhs.headdequeu&&headblock==rhs.headblock&&pos==rhs.pos);
            }

            bool operator!=(const const_iterator &rhs) const {
                return  !(headdequeu==rhs.headdequeu&&headblock==rhs.headblock&&pos==rhs.pos);
            }
        };

        class const_iterator {
            friend class iterator;

            friend class deque;
            // it should has similar member method as iterator.
            //  and it should be able to construct from an iterator.
        private:
            // data members.
        public:
            const_iterator() {
                // TODO
            }

            const_iterator(const const_iterator &other) {
                // TODO
            }

            const_iterator(const iterator &other) {
                // TODO
            }
            // And other methods in iterator.
            // And other methods in iterator.
            // And other methods in iterator.
        };

        /**
         * TODO Constructors
         */
        deque() {
            num = 0;
            head = new block;
            tail = new block;
            head->nxt = tail;
            tail->pre = head;
        }

        deque(const deque &other) {
            //cout<<other.num<<"*** in dequeu copy"<<endl;
            num = other.num;
            head = new block(*other.head);
            tail = new block(*other.tail);
            head->nxt=tail;
            tail->pre=head;
            block *cur=head;
            block*p=other.head;
            p=p->nxt;
            int cnt=0;
            while (p!=other.tail){
             // cout<<++cnt<<endl;
                block *q=new block(*p);
                q->pre=cur;
                cur->nxt=q;
                cur=cur->nxt;
                p=p->nxt;
            }
            tail->pre=cur;
            cur->nxt=tail;
























//            block *p = head;
//            block *q = head->nxt;
//            while (q != other.tail) {
//                p->nxt = new block(*q);
//                p->nxt->nxt = p->nxt;
//                p->nxt->pre = p;
//                p->nxt = q;
//                p->nxt->nxt->pre = q;
//                q = q->nxt;
//                p = p->nxt;
//            }
//            p->nxt = q;
//            q->pre = p;
        }

        /**
         * TODO Deconstructor
         */
        ~deque() {
            block *p = head->nxt;
            block *q;
            head->nxt = tail;
            tail->pre = head;
            while (p != tail) {
                q = p;
                delete q;
                p = p->nxt;
            }
            num = 0;
            delete head;
            delete tail;
        }

        /**
         * TODO assignment operator
         */
        deque &operator=(const deque &other) {
            if (this == &other) return *this;
            block *p = head->nxt;
            block *q;
            head->nxt = tail;
            tail->pre = head;
            while (p != tail) {
                q = p;
                delete q;
                p = p->nxt;
            }
            num = other.num;
            delete head, delete tail;








            num = other.num;
            head = new block;
            tail = new block;
            head->nxt=tail;
            tail->pre=head;
            block *cur=head;
           p=other.head;
            p=p->nxt;
            while (p!=other.tail){
                block *newq=new block(*p);
                newq->pre=cur;
                cur->nxt=newq;
                cur=cur->nxt;
                p=p->nxt;
            }
            tail->pre=cur;
            cur->nxt=tail;
//            head = new block(*other.head);
//            tail = new block;
//            p = head;
//            q = head->nxt;
//            while (q != other.tail) {
//                p->nxt = new block(*q);
//                p->nxt->nxt = p->nxt;
//                p->nxt->pre = p;
//                p->nxt = q;
//                p->nxt->nxt->pre = q;
//                q = q->nxt;
//                p = p->nxt;
//            }
//            p->nxt = q;
//            q->pre = p;
        }

        /**
         * access specified element with bounds checking
         * throw index_out_of_bound if out of bound.
         */
        T &at(const size_t &pos) {
            int temppos = pos;
            block *p = head->nxt;
            while (p != tail) {
                temppos -= p->currentsize;
                if (temppos <0) break;
                p = p->nxt;
            }
            if (p == tail) { throw index_out_of_bound(); }
            else {
                temppos+=p->currentsize;
                return *(p->element[temppos ]);
            }


        }

        const T &at(const size_t &pos) const {
            int temppos = pos;
            block *p = head->nxt;
            while (p != tail) {
                temppos -= p->currentsize;
                if (temppos < 0) break;
                p = p->nxt;
            }
            if (p == tail) { throw index_out_of_bound(); }
            else {
                temppos+=p->currentsize;
                return *(p->element[temppos ]);
            }
        }

        T &operator[](const size_t &pos) {
            //cout<<pos<<endl;
            int temppos = pos;

            block *p = head->nxt;
            //cout<<p->currentsize<<"&&&"<<endl;
            while (p != tail) {

               // temppos++;
                temppos -= p->currentsize;
                if (temppos <0) break;
                p = p->nxt;
                //cout<<"***"<<endl;
            }
            if (p == tail) { throw index_out_of_bound(); }
            else {
                temppos+=p->currentsize;

                return *(p->element[temppos]);
            }
        }

        const T &operator[](const size_t &pos) const {
            int temppos = pos;
            block *p = head->nxt;
            while (p != tail) {
                temppos -= p->currentsize;
                if (temppos < 0) break;
                p = p->nxt;
            }
            if (p == tail) { throw index_out_of_bound(); }
            else {
                temppos+=p->currentsize;
                return *(p->element[temppos ]);
            }
        }

        /**
         * access the first element
         * throw container_is_empty when the container is empty.
         */
        const T &front() const {
            if (head->nxt = tail) throw container_is_empty();
            // block *p=head->nxt;
            return at(0);
        }

        /**
         * access the last element
         * throw container_is_empty when the container is empty.
         */
        const T &back() const {
            return at(num-1);
        }

        /**
         * returns an iterator to the beginning.
         */
        iterator begin() {
            if (num==0) return end();
            iterator temp;
            temp.pos=0;
            temp.headdequeu= this;
            temp.headblock=head->nxt;
            temp.pos=0;
            return temp;

        }

        const_iterator cbegin() const {
            if (num==0) return cend();
            const_iterator temp;
            temp.headqueu=this;
            temp.headblock=head->nxt;
            temp.pos=0;
            return temp;
        }

        /**
         * returns an iterator to the end.
         */
        iterator end() {
            iterator temp;
            temp.pos=0;
            temp.headblock=tail;
            temp.headdequeu= this;
            return temp;
        }

        const_iterator cend() const {
            const_iterator temp;
            temp.pos=0;
            temp.headblock=tail;
            temp.headdequeu= this;
            return temp;
        }

        /**
         * checks whether the container is empty.
         */
        bool empty() const {
            return num == 0;
        }

        /**
         * returns the number of elements
         */
        size_t size() const {
            return num;
        }

        /**
         * clears the contents
         */
        void clear() {}

        /**
         * inserts elements at the specified locat on in the container.
         * inserts value before pos
         * returns an iterator pointing to the inserted value
         *     throw if the iterator is invalid or it point to a wrong place.
         */
        iterator insert(iterator pos, const T &value) {

            if (this!=pos.headdequeu) throw invalid_iterator();
//            if (pos.headblock->currentsize>SPLITNUM){split(pos.headblock);
//            pos=
//            }
            if (pos.headblock->pre!= nullptr&&pos.pos==0){
                pos.pos=pos.headblock->pre->currentsize;
                pos.headblock=pos.headblock->pre;
            }else if (head->nxt!= nullptr){
                block*p=head->nxt;
                pos.pos=pos-begin();
               // cout<<pos.pos<<"    "<<value<<endl;
                while (pos.pos-p->currentsize>=0&&p!=tail){
                    pos.pos-=p->currentsize;
                    p=p->nxt;
                  //  cout<<pos.pos<<"************"<<endl;
                }
                if (p==head->nxt){
                    pos.headblock=p;
                }
                else{
                    if (p == tail) throw invalid_iterator();
                    pos.headblock = p;

                }
            }
            //cout<<value<<"777777"<<endl;
           // cout<<pos.pos<<"   "<<pos.headblock->currentsize<<endl;
            pos.headblock->add(value,pos.pos);
            int num1=pos-begin();
            if (pos.headblock->currentsize>SPLITNUM){split(pos.headblock);
            pos=begin()+num1;
            }
            num++;
           // pos.headdequeu->debug();
            return pos;
        }

        /**
         * removes specified element at pos.
         * removes the element at pos.
         * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
         * throw if the container is empty, the iterator is invalid or it points to a wrong place.
         */
        iterator erase(iterator pos) {
        
        }

        /**
         * adds an element to the end
         */
        void push_back(const T &value) {
            block *p = tail->pre;
            if (p==head){
                block *q=new block;
                p->nxt=q;
                q->pre=p;
                q->nxt=tail;
                tail->pre=q;
                p=q;
            }//todo
            int num1=p->currentsize;
           // cout<<num1<<"  &&&  "<<endl;
            p->element[num1]= new T(value);
            p->currentsize++;
            num++;
            if (p->currentsize > SPLITNUM) {
                //if (p->nxt==tail) cout<<"&&&&"<<endl;
                split(p);
            }
            //cout<<value<<"  ***"<<endl;
        }
        void debug(){
            cout<<"in debug........."<<endl;
            block *p=head->nxt;
            while (p!=tail){
                for (int i = 0; i < p->currentsize; ++i) {
                    cout<<*(p->element[i])<<endl;
                }
                cout<<"****************************"<<endl;
                p=p->nxt;
            }
            cout<<" at the end of debug......."<<endl;
        }

        /**
         * removes the last element
         *     throw when the container is empty.
         */
        void pop_back() {
            if (num == 0) throw container_is_empty();
            block *p = tail->pre;
            while (1) {
                if (p->currentsize != 0) break;
                p = p->pre;
            }
            p->element[p->currentsize - 1] = nullptr;
            p->currentsize--;
            num--;
        }

        /**
         * inserts an element to the beginning.
         */
        void push_front(const T &value) {
            block *p = head->nxt;
            while (p!=tail) {
                if (p->currentsize != 0) break;
                p = p->nxt;
            }
            if (p==tail){
                block *q=new block;
                q->nxt=p;
                q->pre=p->pre;
                p->pre->nxt=q;
                p->pre=q;
                p=q;
            }
            p->add(value, 1);
            if (p->currentsize > SPLITNUM) {
                split(p);
            }
            num++;
        }

        /**
         * removes the first element.
         *     throw when the container is empty.
         */
        void pop_front() {
            if (num == 0) throw container_is_empty();
            block *p = tail->nxt;
            while (1) {
                if (p->currentsize != 0) break;
                p = p->nxt;
            }
            p->erase(1);
            num--;
        }
    };

}

#endif
