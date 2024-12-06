// asignacion de variables
// botones
const int buttonOpen = 8;
const int buttonClose = 9;
bool buttonState = false;

// final carril 
const int finalCarreraOpen = 10;
const int finalCarreraClose =11;
bool finalStateO = false;
bool finalStateC = false;

// motor
const int pinENA = 5;
const int pinIN1 = A2;
const int pinIN2 = A3;

void setup()
{
    // monitor
    Serial.begin(115200);
    // entradas
    pinMode(buttonOpen, INPUT_PULLUP);
    pinMode(buttonClose, INPUT_PULLUP);
    pinMode(finalCarreraOpen, INPUT_PULLUP);
    pinMode(finalCarreraClose, INPUT_PULLUP);

    // salidas
    pinMode(pinENA,OUTPUT);
    pinMode(pinIN1,OUTPUT);
    pinMode(pinIN2,OUTPUT);
    Serial.println("Inicia puerta");
}
void loop()
{
    int stateF1 = digitalRead(finalCarreraOpen);
    int stateF2 = digitalRead(finalCarreraClose);
    // botones
    if (digitalRead(buttonOpen) == LOW){
        buttonState = true;
    }
    if (digitalRead(buttonClose) == LOW){
        buttonState = false;
    }

    // final de carrera   
    if (stateF1 == LOW){
        finalStateO = true;
        finalStateC = false;
    }
    if (stateF2 == LOW){
        finalStateC = true;
        finalStateO = false;
    }
    // logica
    if(buttonState){
        Serial.print("Boton Abierto: ");
        if(finalStateO){
            open();
        }
        if (finalStateC){
            stop();
        }
    }else{
        Serial.print("Boton cerrado: ");
        if(finalStateC){
            close();
        }
        if(finalStateO){
            stop();
        }
    }
}

void open(){
    Serial.println("encendido");
    digitalWrite(pinIN1, LOW);
    digitalWrite(pinIN2, HIGH);
    digitalWrite(pinENA, HIGH);
}
void close(){
    Serial.println("apagado");
    digitalWrite(pinIN1, HIGH);
    digitalWrite(pinIN2, LOW);
    digitalWrite(pinENA, HIGH);
}
void stop(){
    Serial.println("detenido");
    digitalWrite(pinIN1, LOW);
    digitalWrite(pinIN2, LOW);
    digitalWrite(pinENA, LOW);
}