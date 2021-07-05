#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <limits>
#include <algorithm>
using namespace std;
int check;
int a,b,c,d,e,f;

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

static int player;//傳入的玩家
static int opponent;//傳入玩家的對手
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

//void algorithm_A(std::ofstream& fout)
//{
    
    class TreeNode
    {
    private:

        int index[2];//x,y
    public:
        int score = 0;
         std::array<std::array<int, SIZE>, SIZE> boardState ;//這個node自己的棋盤
        std::vector<Point> next_spots;
        std::vector<Point> opp_next_spots;
        std::array<int, 3> disc_count;
        int cur_player;
       
        TreeNode();
        TreeNode(std::array<std::array<int, SIZE>, SIZE> board, int x, int y,int role){//下了x,y之後的board
//            this->boardState  = board;
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; j++) {
                  boardState[i][j] =  board[i][j] ;
                }
            }
            cur_player = role;
            index[0] = x;
            index[1] = y;
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; j++) {
                    if( boardState[i][j]==1){
                        disc_count[1] ++;
                    }
                    else if(boardState[i][j]==2){
                        disc_count[2] ++;
                    }
                }
            }
            disc_count[0] = 64-disc_count[1]-disc_count[2];
//            score = disc_count[player] - disc_count[get_next_player(player)];
        };

        int getX()
        {
            return this->index[0];
        };
        int getY()
        {
            return this->index[1];
        };
        int get_next_player(int player) const {
            return 3 - player;
        }
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
        std::vector<Point> get_opponent_spots() const {
            std::vector<Point> valid_spots;
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; j++) {
                    Point p = Point(i, j);
                    if (boardState[i][j] != 0)
                        continue;
                    if (is_opponent_valid(p))
                        valid_spots.push_back(p);
                }
            }
            return valid_spots;
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
//                int nextplayer = 3-cur_player;
                if (!is_disc_at(p, get_next_player(cur_player)))//上面不是敵人旗子
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
         bool is_opponent_valid(Point center) const {//找下一個可嚇得點
                    if (get_disc(center) != 0)//不是空的不能下
                        return false;
                    for (Point dir: directions) {
                        // Move along the direction while testing.
                        Point p = center + dir;
        //                int nextplayer = 3-cur_player;
                        if (!is_disc_at(p, get_next_player(3-cur_player)))//上面不是敵人旗子
                            continue;//繼續找
                        p = p + dir;
                        while (is_spot_on_board(p) && get_disc(p) != 0) {//上面有棋子
                            if (is_disc_at(p, 3-cur_player))
                                return true;
                            p = p + dir;
                        }
                    }
                    return false;
                }
        void flip_discs(Point center) {//下的那顆旗子
            for (Point dir: directions) {
                // Move along the direction while testing.
                Point p = center + dir;
                if (!is_disc_at(p, get_next_player(cur_player)))
                    continue;
                std::vector<Point> discs({p});
                p = p + dir;
                while (is_spot_on_board(p) && get_disc(p) != 0) {
                    if (is_disc_at(p, cur_player)) {
                        for (Point s: discs) {
                            set_disc(s, cur_player);
                        }
                        disc_count[cur_player] += discs.size();
                        disc_count[get_next_player(cur_player)] -= discs.size();
                        break;
                    }
                    discs.push_back(p);
                    p = p + dir;
                }
            }
        }
                    
        bool put_disc(Point p) {

            set_disc(p, cur_player);
            disc_count[cur_player]++;
            disc_count[0]--;
            flip_discs(p);
//             Give control to the other player.
            cur_player = get_next_player(cur_player);
            next_spots = get_valid_spots();
            opp_next_spots = get_opponent_spots();
//            // Check Win
//
            return true;
        }
    };
            int calculate(TreeNode* node){
                    array<array<int, SIZE>, SIZE> thisboard = node->boardState;
                     int mypoint = node->disc_count[player];
                     int empty = node->disc_count[0];
                     float borderratio, spotratio;
                     empty+=36;
                     mypoint*=100;//1.
                    
                     int dx[2] = {0,7};
                     int dy[2] = {0,7};
                     
                     
                    int mycorner = 0;
                    int oppcorner = 0;
                     
                     if(thisboard[dx[0]][dy[0]]==player){
                         mycorner++;
                     }else if(thisboard[dx[0]][dy[0]]==opponent){
                         oppcorner++;
                     }
                     if(thisboard[dx[0]][dy[1]]==player){
                                mycorner++;
                            }else if(thisboard[dx[0]][dy[1]]==opponent){
                                oppcorner++;
                            }
                     if(thisboard[dx[1]][dy[0]]==player){
                                mycorner++;
                            }else if(thisboard[dx[1]][dy[0]]==opponent){
                                oppcorner++;
                            }
                     if(thisboard[dx[1]][dy[1]]==player){
                                mycorner++;
                            }else if(thisboard[dx[1]][dy[1]]==opponent){
                                oppcorner++;
                            }
                     int cornerdif = mycorner - oppcorner;
                     
                     cornerdif*=12500;//2.

                      int  opp_nextto_corner = 0;
                      int k = node->opp_next_spots.size();
                              for(int i = 0; i < k; i++){
                                  Point p = node->opp_next_spots[i];
                                  if((p.x==0&&p.y==0)||(p.x==0&&p.y==7)||(p.x==7&&p.y==0)||(p.x==7&&p.y==7)){
                                      opp_nextto_corner++;
                                  }
                              }
                      opp_nextto_corner*=5000;//3.

                         int myborder = 0;
                        int oppborder = 0;
                 //        int test1=0,test2=0;
                         for(int i = 1;i < 7;i++){
                             if(thisboard[0][i]==player){
                                 myborder++;
                             }else if(thisboard[0][i]==opponent){
                                 oppborder++;
                             }
                         }
                         for(int i = 1;i < 7;i++){
                             if(thisboard[i][0]==player){
                                 myborder++;
                             }else if(thisboard[i][0]==opponent){
                                 oppborder++;
                             }
                         }
                         for(int i = 1;i < 7;i++){
                             if(thisboard[7][i]==player){
                                 myborder++;
                             }else if(thisboard[7][i]==opponent){
                                 oppborder++;
                             }
                         }
                         for(int i = 1;i < 7;i++){
                             if(thisboard[i][7]==player){
                                 myborder++;
                             }else if(thisboard[i][7]==opponent){
                                 oppborder++;
                             }
                         }
                         myborder+=mycorner;
                         oppborder+=oppcorner;
                int allborder=myborder+oppborder;
                     if(allborder>0){
                         borderratio = 100.0*myborder/allborder;
                         borderratio*=5;//4.
                     }
                         else{
                             borderratio=0;
                         }

                int allnextspot = (node->next_spots.size() + node->opp_next_spots.size());
                    if(allnextspot>0){
                         spotratio = 100.0*node->next_spots.size()/allnextspot;
                                spotratio*=empty;//5.
                        }else{
                                spotratio=0;
                        }
//                spotratio*=100;
                
                int finalpoint = mypoint + borderratio + spotratio + cornerdif - opp_nextto_corner;
 //
                return  finalpoint;
            }

                int minimax(TreeNode* Node, int depth, bool isMax,float record,float alpha,float beta){
        //            int score = point(Node);
                    if(depth == 3){
                        return record;
                    }
                    // If this maximizer's move
                    if (isMax)//ismax=true換我
                    {
                        float bestval = -100000;
                        float moveval = 0;
                        // Traverse all cells
                        int n_valid_spots = (Node->next_spots).size();
                        if(n_valid_spots==0){
                            
                            if(Node->disc_count[player]-Node->disc_count[opponent]>0){
                                return 88888;
                            }else{
                            return -88888;//因為如果第一層只剩一點可下 要回傳-999比第一層bestval3大 才會選這個回傳的值
                            }
                        }
                        for(int i=0;i<n_valid_spots;i++){//走訪可走的點
                            Point p = Node->next_spots[i];
                            //下棋 建立newstate
                            array<array<int, SIZE>, SIZE> now = Node->boardState ;
                            TreeNode *newNode = new TreeNode(now, p.x, p.y,player);//下了其之後變成的node
        //                    newNode->cur_player = Node->cur_player;
                            newNode->put_disc(p);
                            
//                            if((p.x==1&&p.y==1)||(p.x==1&&p.y==6)||(p.x==6&&p.y==1)||(p.x==6&&p.y==6)){
//                                record+=3000;
//                            }//3贏4輸
                          
                            newNode->score = newNode->disc_count[player]-newNode->disc_count[opponent];
//                           if((p.x==1&&p.y==1)||(p.x==1&&p.y==6)||(p.x==6&&p.y==1)||(p.x==6&&p.y==6)){
//                               record+=3;
//                           }
                            record+= 0.5*calculate(newNode);
//                            if((p.x==0&&p.y==0)||(p.x==0&&p.y==7)||(p.x==7&&p.y==0)||(p.x==7&&p.y==7)){
//                                record+=2*( newNode->disc_count[player]-newNode->disc_count[opponent]);
//                                                                                  }
                            moveval = minimax(newNode, depth+1, false,record,alpha,beta);
                            record = 0;
                                if(moveval>bestval){
//
                                    bestval = moveval;
            
                            }
                            if(moveval>alpha){
                                alpha = moveval;
                            }
                            if (beta <= alpha){
                                break;
                            }
                            
                        }
                       
                        return bestval;
                    }
                
                 
                    // If this minimizer's move
                    else
                    {
                        float bestval2 = 100000;
                        float moveval = 0;
                        // Traverse all cells
                        int n_valid_spots = (Node->next_spots).size();
                        if(n_valid_spots==0){
                            if(depth==2){
                            if(Node->disc_count[player]-Node->disc_count[opponent]>0){
                                return 99999;
                            }else{
                            return -99999;
                            }
                            }else{
                               if(Node->disc_count[player]-Node->disc_count[opponent]>0){
                                    return record;
                                }else{
                                return record ;
                            }
                        }
                        }
   
                        for(int i=0;i<n_valid_spots;i++){//走訪可走的點
                        Point p = Node->next_spots[i];
                        //下棋 建立newstate
                        array<array<int, SIZE>, SIZE> now = Node->boardState ;
                        TreeNode *newNode = new TreeNode(now, p.x, p.y,opponent);
       
                        newNode->put_disc(p);
                        newNode->score = newNode->disc_count[player]-newNode->disc_count[opponent];
                       
                        if(depth==0){
                            record+= 0.8*calculate(newNode);
                            
                            
//                            if((p.x==0&&p.y==0)||(p.x==0&&p.y==7)||(p.x==7&&p.y==0)||(p.x==7&&p.y==7)){
//                                return -9999;
//                                                   }//5輸
                            }
                        else if(depth==2){
                            record+= 0.2*calculate(newNode);
//                            if((p.x==1&&p.y==1)||(p.x==1&&p.y==6)||(p.x==6&&p.y==1)||(p.x==6&&p.y==6)){
//                                record+=1*( newNode->disc_count[player]-newNode->disc_count[opponent]);
//                                                   }//5輸
                            }
//                            if((p.x==1&&p.y==1)||(p.x==1&&p.y==6)||(p.x==6&&p.y==1)||(p.x==6&&p.y==6)){
//                                record+=3000;
//                            }//3贏4輸
                    
                            
                        
                        
                        
                        moveval = minimax(newNode,depth+1, true,record,alpha,beta);
                        record = 0;
                        if(moveval<bestval2){
                            bestval2 = moveval;
                            }
                        if(moveval<beta){
                            beta = moveval;
                            }
                        if (beta <= alpha){
                            break;
                            }
           
                        }
                        return bestval2;
                    }
                }
   
        

    
        void firstPlayer(std::ofstream& fout){
            float bestval3 = -150000;
            float alpha = -150000;
            float beta = 150000;
            float moveval = 0;
            Point bestpoint(-1,-1);
//            Point q(-1,-1);
            int cnt=0;
            int n_valid_spots = next_valid_spots.size();
            for(int i=0;i<n_valid_spots;i++){
                Point p = next_valid_spots[i];
                if((p.x==1&&p.y==1)||(p.x==1&&p.y==6)||(p.x==6&&p.y==1)||(p.x==6&&p.y==6)){
                    cnt++;
                }
            }
            
            for(int i=0;i<n_valid_spots;i++){//走訪可走的點
                Point p = next_valid_spots[i];
                //下棋 建立newstate
                TreeNode *newNode = new TreeNode(board, p.x, p.y,player);//下了其之後變成的node
                newNode->put_disc(p);
                
                float record = 1.0*calculate(newNode);
                if((p.x==0&&p.y==0)||(p.x==0&&p.y==7)||(p.x==7&&p.y==0)||(p.x==7&&p.y==7)){
                    bestpoint=p;
                    break;
                }

                if((p.x==1&&p.y==1)||(p.x==1&&p.y==6)||(p.x==6&&p.y==1)||(p.x==6&&p.y==6)){
                    record-=99999;//用這個先手可全贏 但用break+上面return改掉後手4可以贏比較多
                }
                
//
//
                newNode->score = newNode->disc_count[player]-newNode->disc_count[opponent];
                
                
                moveval = minimax(newNode,0,false,record,alpha,beta);
                record = 0;
                
                
                if(moveval>bestval3){
                    bestpoint = p;
                    bestval3 = moveval;
                }
                if(moveval>alpha){
                    alpha = moveval;
                }
                if (beta <= alpha){
                    break;
                }
            }

            fout << bestpoint.x << " " << bestpoint.y <<std::endl;
            fout.flush();
          
        }

int main(int, char** argv) {
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);
    read_valid_spots(fin);
    firstPlayer(fout);
//    write_valid_spot(fout);
    fin.close();
    fout.close();
    return 0;
}



