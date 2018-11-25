#include "mbed.h"
#include "Motor.h"

Motor::Motor(PinName p01,PinName p10,PinName p02,PinName p20,PinName p03,PinName p30):
_p01(p01),_p10(p10),_p02(p02),_p20(p20),_p03(p03),_p30(p30)
{
    _p01 = 0;
    _p10 = 0;
    _p02 = 0;
    _p20 = 0;
    _p03 = 0;
    _p30 = 0;
}

/*モーター周波数変更初期設定*/
void Motor::setPwmPeriod(float hz){
    _p01.period(hz);
    _p10.period(hz);
    _p02.period(hz);
    _p20.period(hz);
    _p03.period(hz);
    _p30.period(hz);
}

/*モーター制御*/
void Motor::setPower(float a,float b,float c){
    
    a = a / 100;
    b = b / 100;
    c = c / 100;
    
    if(a > 0) { //正回転
        _p01 = a;
        _p10 = 0;
    } else if(a == 0) { //ブレーキ
        _p01 = 1;
        _p10 = 1;//0か1だと思う
    } else { //負回転
        _p01 = 0;
        _p10 = -1*a;
    }
    if(b > 0) {
        _p02 = b;
        _p20 = 0;
    } else if(b == 0) {
        _p02 = 1;
        _p20 = 1;
    } else {
        _p02 = 0;
        _p20 = -1*b;
    }
    if(c > 0) {
        _p03 = c;
        _p30 = 0;
    } else if(c == 0) {
        _p03 = 1;
        _p30 = 1;
    } else {
        _p03 = 0;
        _p30 = -1*c;

    }
}

//*********************************************************************************/////////
///////モーター制御θ(degree)に進行方向,powerにPWMの値(0～100),修正値をmodに代入,右回転基準//////////
//********************************************************************************://///////
/*3輪用*/
void Motor::omniWheels(int degree,int power,int mod){
    float motor[3];//モーター用変数
    float Max[2];//最大値用変数
    
    if(power == 0){//パワー0のとき
        
        motor[0] = mod; //モーター右
        motor[1] = mod; //モーター後
        motor[2] = mod; //モーター左
    }
    else{ 
    
        motor[0] = sin((degree-60)*Pi/180) + (float)mod * 0.01; //モーター右
        motor[1] = sin((degree-180)*Pi/180) + (float)mod * 0.01; //モーター後
        motor[2] = sin((degree-300)*Pi/180) + (float)mod * 0.01; //モーター左
        
        if(motor[0]>1){
            motor[0] = 1;
        }
        else if(motor[0] < -1){
            motor[0] = -1;
        }
        if(motor[1]>1){
            motor[1] = 1;
        }
        else if(motor[1] < -1){
            motor[1] = -1;
        }
        if(motor[2]>1){
            motor[2] = 1;
        }
        else if(motor[2] < -1){
            motor[2] = -1;
        }
        
    
        if(fabs(motor[0]) >= fabs(motor[1])){ //モーターパワー最大値計算
            Max[0] = fabs(motor[0]);
        }
        else{
            Max[0] = fabs(motor[1]);
        }
        if(fabs(motor[2]) >= Max[0]){
            Max[1] = fabs(motor[2]);
        }
        else{
            Max[1] = Max[0];//最大値
        }
        
    
        motor[0] = (power*(motor[0]/Max[1]));//モータパワー最大値修正
        motor[1] = (power*(motor[1]/Max[1]));
        motor[2] = (power*(motor[2]/Max[1]));
    }
    
    setPower(motor[0],motor[1],motor[2]);
    
}

