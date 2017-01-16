#include "mbed.h"
#include "m3pi.h"
 
m3pi m3pi;

// Déclaration des pin d'entree
DigitalIn In1(p15);
DigitalIn In2(p16);
DigitalIn In3(p17);

// Déclaration des pin de sortie
DigitalOut Out1(p18);
DigitalOut Out2(p19);
DigitalOut Out3(p20);



// Vitesse maximale et vitesse minimum
#define MAX 0.3
#define MIN 0
 
// PID terms
#define P_TERM 1
#define I_TERM 0
#define D_TERM 20
 

// Permet de convertir les opt sortie en un chifre
int getInput(int in1 , int in2 , int in3)
{
    int decimalNumber = 0;
    
    decimalNumber += pow(2.0,0.0) * in1;
    decimalNumber += pow(2.0,1.0) * in2;
    decimalNumber += pow(2.0,2.0) * In3;
        
    return decimalNumber;
}

void set_output( int nb){
    
    
    Out1 = 0;
    Out2 = 0;
    Out3 = 0;

    if( nb > 7 || nb < 1)
        return;
  
    if( nb % 2 == 1)
        Out1 = 1;
    
    nb = (int) nb/2;
    if( nb < 1) 
        return;
        
    if( nb % 2 == 1)
        Out2 = 1;
        
    nb = (int) nb/2;
    if( nb < 1)
        return;

    if( nb % 2 == 1)
        Out3 = 1;
        
}

int main() {
 
    float right;
    float left;
    float direction;
    float current_pos_of_line = 0.0;
    float previous_pos_of_line = 0.0;
    float derivative,proportional,integral = 0;
    float power;
    float speed = MAX;
    
    bool manualControl = true;
 
        /*
    Out1 = 0;
    Out2 = 0;
    Out3 = 0;
    */

    
    m3pi.locate(0,1);
    m3pi.printf("Owaa v1");
 
    wait(2.0);
    m3pi.sensor_auto_calibrate();
    //clock_t t =  clock();
    int number = 0;
    while (1) {
        
        
        number = getInput ( In1 , In2 , In3);
        /*
         char buf[10];
        
        sprintf(buf, "%d", number);
        m3pi.locate(0,1);
        m3pi.printf("        " );
        m3pi.locate(0,1);
        m3pi.printf(buf );
        */
        if( number == 5){
            manualControl = true;
        }
        else if( number == 6) {
            manualControl = false;
            
            current_pos_of_line = 0.0;
            previous_pos_of_line = 0.0;
            derivative = 0;
            proportional = 0;
            integral = 0;
            
        }/*
        else if (number == 7){
            
            m3pi.stop();
            continue;
        }
            */
            
        if( manualControl){
     
              //   set_output(0);

                     //       m3pi.locate(0,1);

                   // m3pi.printf("Manual" );

            switch(number){
                
                case 0: // Anvance
                    m3pi.stop();
                break;
                    
                case 1: // Anvance
                    m3pi.backward(0);
                    m3pi.forward(speed);     
                break;
                
                case 2: // Recule
                    m3pi.forward(0);
                    m3pi.backward(speed);
                break;
                
                case 3: // Droite
                m3pi.left_motor(-(speed/3));
                m3pi.right_motor(speed/3);
                break;
                
                case 4: // Gauche
                m3pi.left_motor(speed/3);
                m3pi.right_motor(-( speed/3));
                break;
                
                case 7: // Stop
                    m3pi.stop();
                break;
                
            }// Fin switch
            
            
        }//Fin If mannual
        
        
        else{
            
         //m3pi.locate(0,1);
        // m3pi.printf("Auto" );

 
        // Get the position of the line.
        current_pos_of_line = m3pi.line_position();        
        proportional = current_pos_of_line;
        
        // Compute the derivative
        derivative = current_pos_of_line - previous_pos_of_line;
        
        // Compute the integral
        integral += proportional;
        
        // Remember the last position.
        previous_pos_of_line = current_pos_of_line;
        
        // Compute the power
        power = (proportional * (P_TERM) ) + (integral*(I_TERM)) + (derivative*(D_TERM)) ;
        
        // Compute new speeds   
        right = speed+power;
        left  = speed-power;
        
      //  m3pi.locate(0,1);
        
        /*
        if( tourne){
            
            m3pi.stop();
            tourne = false;
            m3pi.printf("P" );
            wait(2.0);
                
            
            
        }
*/

        // limit checks
        
        direction = right - left;  
    
        
        if (right < MIN)
            right = MIN;
        else if (right > MAX)
            right = MAX;
            
        if (left < MIN)
            left = MIN;
        else if (left > MAX)
            left = MAX;
            
       // set speed 
        m3pi.left_motor(left);
        m3pi.right_motor(right);
        
       if( direction > -0.6 && direction < 0.6){
            set_output(3);
        
        }
            
        else if ( direction > -0.6){
         
            set_output(5); 
           

        }
        else if (direction < 0.6 ){
            set_output(6);

        }
        
         
        }
 
    }
}

