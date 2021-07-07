#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cassert>
using namespace std;

struct Point {
    int x, y;
    Point() : Point(0, 0) {}
    Point(int x, int y) : x(x), y(y) {}
    Point(float x, float y) : x(x), y(y) {}
    bool operator==(const Point& rhs) const {
        return x == rhs.x && y == rhs.y;
    }
    bool operator!=(const Point& rhs) const {
        return !operator==(rhs);
    }
    Point operator+(const Point& rhs) const {
        return Point(x + rhs.x, y + rhs.y);
    }
    Point operator-(const Point& rhs) const {
        return Point(x - rhs.x, y - rhs.y);
    }
};
static const int SIZE = 8;
const std::array<Point, 8> directions{{//flip disc用
    Point(-1, -1), Point(-1, 0), Point(-1, 1),
    Point(0, -1), /*{0, 0}, */Point(0, 1),
    Point(1, -1), Point(1, 0), Point(1, 1)
}};



int player;//傳入的玩家
int opponent;//傳入玩家的對手
//const int SIZE = 8;
std::array<std::array<int, SIZE>, SIZE> board;//傳入的棋盤
std::vector<Point> next_valid_spots;//下一個可下的點

void read_board(std::ifstream& fin) {
    fin >> player;
    opponent = 3-player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> board[i][j];
        }
    }
}

void read_valid_spots(std::ifstream& fin) {
    int n_valid_spots;
    fin >> n_valid_spots;
    int x, y;
    for (int i = 0; i < n_valid_spots; i++) {
        fin >> x >> y;
        next_valid_spots.push_back({x, y});
    }
}

void write_valid_spot(std::ofstream& fout) {
    int n_valid_spots = next_valid_spots.size();
    srand(time(NULL));
    // Choose random spot. (Not random uniform here)
    int index = (rand() % n_valid_spots);
    Point p = next_valid_spots[index];
    // Remember to flush the output to ensure the last action is written to file.
    fout << p.x << " " << p.y << std::endl;
    fout.flush();
}


void algorithm_A(std::ofstream& fout)
{
    
    class TreeNode
    {
    private:
//        std::array<std::array<int, SIZE>, SIZE> boardState;//這個node自己的棋盤
        TreeNode *nextfirstNode;//四個node list
        TreeNode *nextsecondNode;
        TreeNode *nextthirdNode;
        TreeNode *nextfourthNode;

        int index[2];//x,y
    public:
//        int get_next_player(int player) const {
//            return 3 - player;
//        }
        int score=100;
        int mynum;
        int hisnum;
        std::array<int, 3> disc_count  = {};
        std::vector<Point> next_spots;
        int cur_player ;
        std::array<std::array<int, SIZE>, SIZE> boardState ;//這個node自己的棋盤
        TreeNode();
        TreeNode(std::array<std::array<int, SIZE>, SIZE> board)
        {
//            this->next_spots = next_valid_spots;;
            this->boardState = board;
            nextfirstNode = NULL;
            nextsecondNode = NULL;
            nextthirdNode = NULL;
            nextfourthNode = NULL;
            index[0] = 0;
            index[1] = 0;
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; j++) {
                    if( board[i][j]==1){
                        disc_count[1] ++;
                    }
                    else if(board[i][j]==2){
                        disc_count[2] ++;
                    }
                }
            }
            disc_count[0] = 64-disc_count[1]-disc_count[2];
            
        };
        TreeNode(std::array<std::array<int, SIZE>, SIZE> board, int x, int y)
        {//下了x,y之後的board
            this->boardState  = board;
            nextfirstNode = NULL;
            nextsecondNode = NULL;
            nextthirdNode = NULL;
            nextfourthNode = NULL;
            this->index[0] = x;
            this->index[1] = y;
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; j++) {
                    if( board[i][j]==1){
                        disc_count[1] ++;
                    }
                    else if(board[i][j]==2){
                        disc_count[2] ++;
                    }
                }
            }
            disc_count[0] = 64-disc_count[1]-disc_count[2];
        };
        void setNextfirstNode(TreeNode *Node)
        {
            this->nextfirstNode = Node;
        };
        void setNextsecondNode(TreeNode *Node)
        {
            this->nextsecondNode = Node;
        };
        void setNextthirdNode(TreeNode *Node)
        {
            this->nextthirdNode = Node;
        };
        void setNextfourthNode(TreeNode *Node)
        {
            this->nextfourthNode = Node;
        };
        TreeNode *getNextfirstNode()
        {
            return this->nextfirstNode;
        };
        TreeNode *getNextsecondNode()
        {
            return this->nextsecondNode;
        };
        TreeNode *getNextthirdNode()
        {
            return this->nextthirdNode;
        };
        TreeNode *getNextfourthNode()
        {
            return this->nextfourthNode;
        };
        TreeNode *getfirstTail()
        {
            TreeNode *temp = NULL;
            TreeNode *tempNext = this;
            while (tempNext != NULL)
            {
                temp = tempNext;
                tempNext = tempNext->getNextfirstNode();
            }
            return temp;
        };
        TreeNode *getsecondTail()
        {
            TreeNode *temp = NULL;
            TreeNode *tempNext = this;
            while (tempNext != NULL)
            {
                temp = tempNext;
                tempNext = tempNext->getNextsecondNode();
            }
            return temp;
        };
        TreeNode *getthirdTail()
        {
            TreeNode *temp = NULL;
            TreeNode *tempNext = this;
            while (tempNext != NULL)
            {
                temp = tempNext;
                tempNext = tempNext->getNextthirdNode();
            }
            return temp;
        };
        TreeNode *getfourthTail()
        {
            TreeNode *temp = NULL;
            TreeNode *tempNext = this;
            while (tempNext != NULL)
            {
                temp = tempNext;
                tempNext = tempNext->getNextfourthNode();
            }
            return temp;
        };
//        Board getBoardState()
//        {
//            return this->boardState;
//        };
        int getX()
        {
            return this->index[0];
        };
        int getY()
        {
            return this->index[1];
        };
        std::vector<Point> get_valid_spots() const {
            std::vector<Point> valid_spots;
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; j++) {
                    Point p = Point(i, j);
                    if (boardState[i][j] != 0)
                        continue;
                    if (is_spot_valid(p))
                        valid_spots.push_back(p);
                }
            }
            return valid_spots;
        }
        void flip_discs(Point center) {//下的那顆旗子
            for (Point dir: directions) {
                // Move along the direction while testing.
                Point p = center + dir;
                if (!is_disc_at(p, 3-cur_player))
                    continue;
                std::vector<Point> discs({p});
                p = p + dir;
                while (is_spot_on_board(p) && get_disc(p) != 0) {
                    if (is_disc_at(p, cur_player)) {
                        for (Point s: discs) {
                            set_disc(s, cur_player);
                        }
                        disc_count[cur_player] += discs.size();
                        disc_count[3-cur_player] -= discs.size();
                        break;
                    }
                    discs.push_back(p);
                    p = p + dir;
                }
            }
        }
        bool is_spot_on_board(Point p) const {//點在範圍之內
            return 0 <= p.x && p.x < SIZE && 0 <= p.y && p.y < SIZE;
        }
        int get_disc(Point p) const {//傳回board上的數字
            return boardState[p.x][p.y];
        }
        void set_disc(Point p, int disc) {//設定board上的數字
            boardState[p.x][p.y] = disc;
        }
        bool is_disc_at(Point p, int disc) const {//point p上是不是黑棋或白棋
            if (!is_spot_on_board(p))
                return false;
            if (get_disc(p) != disc)
                return false;
            return true;
        }
        bool is_spot_valid(Point center) const {//找下一個可嚇得點
            if (get_disc(center) != 0)//不是空的不能下
                return false;
            for (Point dir: directions) {
                // Move along the direction while testing.
                Point p = center + dir;
                int nextplayer = 3-cur_player;
                if (!is_disc_at(p, nextplayer))//上面不是敵人旗子
                    continue;//繼續找
                p = p + dir;
                while (is_spot_on_board(p) && get_disc(p) != 0) {//上面有棋子
                    if (is_disc_at(p, cur_player))
                        return true;
                    p = p + dir;
                }
            }
            return false;
        }
    };

    class game
    {
    private:
        TreeNode *root;
//        int player;
//        int opponent;
        

    public:
        
        game(std::array<std::array<int, SIZE>, SIZE> board)
        {
            this->root = new TreeNode(board);       //傳入的board
//            this->root->cur_player = player;
//            this->player = player;                  //傳入的player//1 or 2
//            char colorPlayer = player->get_color(); //player顏色代號
//            char colorOpponent = 'v';
//            for (int i = 0; i < ROW; i++)
//            {
//                for (int j = 0; j < COL; j++)
//                {
//                    if (board.get_cell_color(i, j) != 'w' && board.get_cell_color(i, j) != colorPlayer)
//                        colorOpponent = board.get_cell_color(i, j); //都設成對手!
//                }
//            }
//            this->opponent = new Player(colorOpponent);
            
//            this->opponent = 3-player;
            
        };
        int get_next_player(int player) const {
            return 3 - player;
        }
        
//        void flip_discs(Point center) {//傳入下的那顆旗子 翻轉棋子結果
//
//        }
           TreeNode *select()
                {
                    this->firstPlayer(this->root);//把可以下的地方的各種結果做成list
                    TreeNode *bestNode = this->root->getNextfirstNode();
        //            float best =this->point(bestNode);
        //            float first;
                    
                    float best = INT_MIN;


                    TreeNode *current = this->root->getNextfirstNode();
                    while (current!= NULL)
                    {

                        
                       
                       
                        this->secondOpponent(current);
                        TreeNode *worstNode = current->getNextsecondNode();
                        float worst = INT_MAX;
                        float second;
                        
                        TreeNode *secondcurrent = current->getNextsecondNode();
                        
        //
                        while (secondcurrent!= NULL)
                        {
        //
        //                    Score += 0.8 * this->point(secondcurrent);
                            second =  this->point(secondcurrent);
                            secondcurrent->score = second;
        //                    this->thirdPlayer(secondcurrent);
        //                    TreeNode* thirdcurrent = secondcurrent->getNextthirdNode();
        //
        //                    while(thirdcurrent->getNextthirdNode()!=NULL){
        //
        //
        //                        Score += 0.7 * this->point(thirdcurrent);
        //
        //                      thirdcurrent=thirdcurrent->getNextthirdNode();
        //
        //                    }
                            if(second < worst){
                                
                                worst = second;
                                worstNode = secondcurrent;
                                
                            }
                            secondcurrent = secondcurrent->getNextsecondNode();
                        }
        //                c = current->getX();//下的位置r,c
        //                r = current->getY();
        //                float first;
        //                first = this->point(current);//新棋盤已經放好了
                        current->score = worst;
                        
                        
        //                first = current->getNextsecondNode();
                        if (worst > best)
                        {
                                best = worst;
                                bestNode = current;
        //                    }
                        }
                        
                        current = current->getNextfirstNode();
                    }
                    
                    return bestNode;
                
                };

        void firstPlayer(TreeNode *Node)//長出第一層樹
        {
            Node->cur_player = player;
            int n_valid_spots = next_valid_spots.size();
            for(int i=0;i<n_valid_spots;i++){
                Point p = next_valid_spots[i];
                
//               array<std::array<int, SIZE>, SIZE> newboardState = board;
                
//               boardState[p.x][p.y] = Node->cur_player;//下棋之後做成boardstate
//                boardState.place_orb(i, j, this->player);//錯在這
                TreeNode *newNode = new TreeNode(board, p.x, p.y);
//                Point newp(p.x,p.y);
                //                center.x = Node->getX();
                //                center.y = Node->getY();
                newNode->cur_player = Node->cur_player;
                newNode->set_disc(p, Node->cur_player);
                newNode->disc_count[Node->cur_player]++;
                newNode->disc_count[0]--;
                newNode->flip_discs(p);
                TreeNode *tail = Node->getfirstTail();
                tail->setNextfirstNode(newNode);
                newNode->setNextfirstNode(NULL);
                
            }
           
          
        };
        void secondOpponent(TreeNode *Node)
        {
            
            Node->cur_player = get_next_player(player);
            Node->next_spots = Node->get_valid_spots();
            int n_valid_spots = Node->next_spots.size();
                    for(int i=0;i<n_valid_spots;i++){
                        Point p = Node->next_spots[i];
                
                        TreeNode *newNode = new TreeNode(Node->boardState, p.x, p.y);
            //                Point newp(p.x,p.y);
                            //                center.x = Node->getX();
                            //                center.y = Node->getY();
                        
                            newNode->cur_player = Node->cur_player;
                            newNode->set_disc(p, Node->cur_player);
                            newNode->disc_count[Node->cur_player]++;
                            newNode->disc_count[0]--;
                            newNode->flip_discs(p);
                            TreeNode *tail = Node->getsecondTail();
                            tail->setNextsecondNode(newNode);
                            newNode->setNextfirstNode(NULL);
                    }
//            Node->cur_player = player;
        };
        void thirdPlayer(TreeNode *Node)
        {
             Node->cur_player = player;
             Node->next_spots = Node->get_valid_spots();
             int n_valid_spots = Node->next_spots.size();
                     for(int i=0;i<n_valid_spots;i++){
                         Point p = Node->next_spots[i];
                 
                         TreeNode *newNode = new TreeNode(Node->boardState, p.x, p.y);
                             newNode->set_disc(p, newNode->cur_player);
                             newNode->disc_count[newNode->cur_player]++;
                             newNode->disc_count[0]--;
                             newNode->flip_discs(p);
                             TreeNode *tail = Node->getthirdTail();
                             tail->setNextthirdNode(newNode);
                             newNode->setNextthirdNode(NULL);
                     }
             Node->cur_player = player;

        };
//         void fourthOpponent(TreeNode *Node)
//        {
//            char oppocolor = this->opponent->get_color();
//
//            for (int i = 0; i < ROW; i++)
//            {
//                for (int j = 0; j < COL; j++)
//                {
//                    if (Node->getBoardState().get_cell_color(i, j) == oppocolor || Node->getBoardState().get_cell_color(i, j) == 'w')
//                    {
//                        Board boardState = Node->getBoardState();//找到現在棋盤
//                        boardState.place_orb(i, j,this->opponent);//錯在這//下棋
//                        TreeNode *newNode = new TreeNode(boardState, i, j);//現在的狀況 下在哪裡傳到下一層
//                        TreeNode *tail = Node->getfourthTail();
//                        tail->setNextfourthNode(newNode);
//                        newNode->setNextfourthNode(NULL);
//
//                    }
//                }
//            }
//        };
        


        float point(TreeNode *Node)
        {
//            array<std::array<int, SIZE>, SIZE> thisboard = Node->boardState;
//            char color = player->get_color();     //player的顏色!
//            char oppocolor = opponent->get_color(); //對手的顏色!
            float playernum = 0, opponentnum = 0;
            float num = 0;
            
           
            playernum = Node->disc_count[player];
            opponentnum = Node->disc_count[get_next_player(player)];
            
            Node->mynum = playernum;
            Node->hisnum = opponentnum;
//
            num = playernum - opponentnum;
            return num;
        }
//        int getnum(){
//
//
//        }
    };

    game newgame(board);

    TreeNode *outcome = newgame.select();
    Point p(outcome->getX(),outcome->getY());
    fout << p.x << " " << p.y <<std::endl;
//    fout << p.x << " " << p.y << " " << outcome->score << " " << outcome->disc_count[1] << " " << outcome->disc_count[2] <<  " " << outcome-> disc_count[0]<<" " <<std::endl;
//     TreeNode* tmp = outcome;
//    while(tmp->getNextfirstNode()!=NULL){
//        fout<<" "<<tmp->getNextfirstNode()->score;
//        tmp = tmp->getNextfirstNode();
//    }
//    for (int i = 0; i < SIZE; i++) {
//        for (int j = 0; j < SIZE; j++) {
//            fout<<outcome->boardState[i][j] ;
//        }
//    }
//    fout <<  std::endl;
    fout.flush();
}


int main(int, char** argv) {
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);
    read_valid_spots(fin);
    algorithm_A(fout);
//    write_valid_spot(fout);
    fin.close();
    fout.close();
    return 0;
}

//TreeNode *select()
//                {
//                    this->firstPlayer(this->root);//把可以下的地方的各種結果做成list
//                    TreeNode *bestNode = this->root->getNextfirstNode();
//        //            float best =this->point(bestNode);
//        //            float first;
//
//                    float best = INT_MIN;
//
//
//                    TreeNode *current = this->root->getNextfirstNode();;
//                    while (current!= NULL)
//                    {
//
//
//
//
//                        this->secondOpponent(current);
//                        TreeNode *worstNode = current->getNextsecondNode();
//                        float worst = INT_MAX;
////                        float second;
//
//                        TreeNode *secondcurrent = current->getNextsecondNode();
//
//        //
//                        while (secondcurrent!= NULL)
//                        {
//        //
//        //                    Score += 0.8 * this->point(secondcurrent);
//
//                            this->thirdPlayer(secondcurrent);
//                            TreeNode *greatNode = secondcurrent->getNextthirdNode();
//                            float great = INT_MIN;
//                            float third;
//                            TreeNode* thirdcurrent = secondcurrent->getNextthirdNode();
//        //
//                            while(thirdcurrent!=NULL){
//
//                                third =  this->point(thirdcurrent);
////                                secondcurrent->score = second;
////                                Score += 0.7 * this->point(thirdcurrent);
//                                if(third > great){
//
//                                        great = third;
//                                        greatNode = thirdcurrent;
//
//                                }
//
//                              thirdcurrent=thirdcurrent->getNextthirdNode();
//
//                            }
//                            if(great < worst){
//
//                                worst = great;
//                                worstNode = secondcurrent;
//
//                            }
//                            secondcurrent = secondcurrent->getNextsecondNode();
//                        }
//        //                c = current->getX();//下的位置r,c
//        //                r = current->getY();
//        //                float first;
//        //                first = this->point(current);//新棋盤已經放好了
//                        current->score = great;
//
//
//        //                first = current->getNextsecondNode();
//                        if (worst > best)
//                        {
//                                best = worst;
//                                bestNode = current;
//        //
//                        }
//
//                        current = current->getNextfirstNode();
//                    }
//
//                    return bestNode;
//
//                };
//
