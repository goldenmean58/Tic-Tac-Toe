#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    this->setMinimumSize(150,150);
    ui->setupUi(this);
}

void Widget::paintEvent(QPaintEvent *event) {
    QPainter *p = new QPainter();
    p->begin(this);
    p->drawLine(0,50,150,50);
    p->drawLine(0,100,150,100);
    p->drawLine(50,0,50,150);
    p->drawLine(100,0,100,150);
    for(int i = 0; i< 3; ++i) {
        for(int j =0 ;j <3 ; ++j ){
            if(map[i][j] == 1) {
                p->drawLine(j * 50,i*50,(j+1) *50,(i+1) * 50);
                p->drawLine((j+1) * 50 ,i*50,j *50,(i+1) * 50);
            } else if(map[i][j] == 2) {
                p->drawEllipse(j * 50 ,i * 50 ,50,50 );
            }
        }
    }
    p->end();
    delete p;
}

int Widget::judge_game_state(int state) {
    if(state == 0) {
        bool is_full = true;
        for(int i = 0; i < 3; ++i) {
            for(int j =0;j<3;++j){
                if(map[i][j] == 0) {
                    is_full = false;
                }
            }
        }
        if(is_full) {
            //draw
            QMessageBox::information(this, "Game over", "Draw!");
            for(int i = 0; i < 3; ++i) {
                for(int j =0;j<3;++j){
                    map[i][j] = 0;
                }
            }
        }
    }
    if(state == -1) {
        QMessageBox::information(this, "Failed to move", "That place is not empty!");
        return -1;
    }
    if(state > 0) {
        QMessageBox::information(this, "Game over", "Player " + QString::number(state) + " won!");
        for(int i = 0; i < 3; ++i) {
            for(int j =0;j<3;++j){
                map[i][j] = 0;
            }
        }
    }
    update();
    return 0;
}

void Widget::mousePressEvent(QMouseEvent *event) {
    if(event->x() > 150 || event->y() > 150)
        return;
    int move =  event->x() / 50 + event->y() / 50 * 3;
    int game_state = scheduler(move);
    int valid = judge_game_state(game_state);
    if(valid == 0) {
        game_state = scheduler(get_best_move(ai(map, 2,7)));
        judge_game_state(game_state);
    }
}

Widget::~Widget()
{
    delete ui;
}
