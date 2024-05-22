
#include <Servo.h>
// ------------------Servo Motores-----------------------
int posicao_servo1 = 0;
int posicao_servo2 = 180;
Servo servo_1;
Servo servo_2;

// ------------------LED RGB-----------------------------
int R = 48;
int G = 46;
int B = 50;
int Y = 52;

// ------------------Motores Rodas-----------------------
int in1 = 8;
int in2 = 4;
int in3 = 6;
int in4 = 3;
int acti=7;

// ------------------Sensores Ultrassônico-----------------
int cm;
byte pinoTransmissor=13; 
byte pinoReceptor=12; 

byte pinoTransmissorDireita=9; 
byte pinoReceptorDireita=7; 

byte pinoTransmissorEsquerda=11; 
byte pinoReceptorEsquerda=10; 

// ------------------Sensor de Cor RGB-------------------
int S0 = 24;
int S1 = 22;
int S2 = 28;
int S3 = 26;
int Out = 34;
int LED = 32;

unsigned int valorVermelho = 100;
unsigned int valorVerde = 0;
unsigned int valorAzul = 0;
unsigned int valorBranco = 0;

// ------------------Definicao das funcoes---------------
void anda_frente();
void anda_para_tras();
void gira_direita();
void gira_esquerda();
void para();
float distancia_direita();
float distancia_esquerda();
float distanci_frente();
int detecta_cor();
void abre_portao();
void fecha_portao();
void anda_frente_sem_correcao();

// ----------------- Distância ate as lixeiras ----------
float distancia_liexeira_metal = 46;
float distancia_liexeira_papel = 53;
float distancia_liexeira_vidro = 83;
float distancia_liexeira_plastico = 30;

int estado = 1;

void setup()
{
  Serial.begin(9600);
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);

  pinMode(acti, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  digitalWrite(acti, HIGH);

  pinMode(pinoTransmissor, OUTPUT); // transmissor
  pinMode(pinoReceptor, INPUT);     // receptor

  pinMode(pinoTransmissorDireita, OUTPUT); // transmissor
  pinMode(pinoReceptorDireita, INPUT);     // receptor

  pinMode(pinoTransmissorEsquerda, OUTPUT); // transmissor
  pinMode(pinoReceptorEsquerda, INPUT);     // receptor

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(Out, INPUT);
  pinMode(LED, OUTPUT);

  // Configura a escala de frequência para 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);

  delay(2000); 
  digitalWrite(LED, HIGH);

  servo_1.attach(5);
  servo_1.write(posicao_servo1);
  servo_2.attach(2);
  servo_2.write(posicao_servo2);
}

void loop()
{
  if (estado == 1){
    delay(3000);
    coleta_material();
    int lixeira = detecta_cor();
    delay(1000);
    vai_para_lixeira(lixeira);
    delay(1000);
    gira_direita();
    delay(900);
    para();
    delay(1000);
    deposita_material();
    delay(1000);
    gira_direita();
    delay(800);
    anda_para_tras();
    estado = 2;
  }
  else{
    para();
  }
}

void gira_direita()
{
  analogWrite(in1, 200);
  digitalWrite(in2, LOW);
  analogWrite(in3, 200);
  digitalWrite(in4, LOW);
}
void para(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
void gira_esquerda(){
  digitalWrite(in1, LOW);
  analogWrite(in2, 200);
  digitalWrite(in3, LOW);
  analogWrite(in4, 200);
}

void anda_frente_sem_correcao(){
  analogWrite(in1, 200);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  analogWrite(in4, 200);
  }
void anda_frente(){
  if (distancia_direita() > distancia_esquerda() && distancia_direita()<42){
    Serial.println(distancia_direita());
    Serial.println(distancia_esquerda());
    Serial.println("Andando pra direita");
    analogWrite(in1, 230);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    analogWrite(in4, 130);
  }
  else if ( distancia_direita() < distancia_esquerda() && distancia_esquerda()<42){
    Serial.println(distancia_direita());
    Serial.println(distancia_esquerda());
    Serial.println("Andando pra esquerda");
    analogWrite(in1, 110);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    analogWrite(in4, 240);
  }
  else{
  analogWrite(in1, 210);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  analogWrite(in4, 210);
  }
}

void anda_para_tras(){
  digitalWrite(in1, LOW);
  analogWrite(in2, 200);
  analogWrite(in3, 200);
  digitalWrite(in4, LOW);
}

void coleta_material(){
  while (distancia_frente() >= 23){
    anda_frente();
    Serial.println("Andando frente");
    Serial.println(distancia_frente());
  }
  para();
  delay (1000);
  Serial.println("Parei");
  abre_portao();
  Serial.println("Abrindo portao");
  delay (1000);
  anda_frente_sem_correcao();
  Serial.println("Coletando material");
  delay(800);
  para();
  fecha_portao();
  delay(1000);
  Serial.println("Fechando portao");
  anda_para_tras();
  delay(800);
  para();
}

void deposita_material(){
  while (distancia_frente() >= 23){
    anda_frente_sem_correcao();
    Serial.println("Andando frente");
    Serial.println(distancia_frente());
  }
  para();
  delay(1000);
  Serial.println("Parei");
  abre_portao();
  Serial.println("Abrindo portao");
  delay(1000);
  anda_para_tras();
  Serial.println("Depositando material");
  delay(1000);
  para();
  fecha_portao();
  Serial.println("Fechando portao");
  delay(1000);
  }

void vai_para_lixeira(int lixeira){
  if (lixeira == 1){
    //lixeira de plastico
    while (distancia_frente()<distancia_liexeira_plastico){
      anda_para_tras();
    }
  }
  else if (lixeira == 2){
    //lixeira de papel
    while (distancia_frente()<distancia_liexeira_papel){
      anda_para_tras();
    }
  }
  else if (lixeira == 3){
    //lixeira de metal
    while (distancia_frente()<distancia_liexeira_metal){
      anda_para_tras();
    }
  }
  else if (lixeira == 4){
    //lixeira de vidro
    while (distancia_frente()<distancia_liexeira_vidro){
      anda_para_tras();
    }
  }
  para();
}

void anda_pelo_ambiente(){
  if (distancia_frente() >= 12){
    anda_frente();
    Serial.println("Andando frente");
    Serial.println(distancia_frente());
  }
  else if (distancia_direita() < 10 && distancia_esquerda() < 10){
      anda_para_tras();
      Serial.println("Andando tras");
      Serial.println(distancia_frente());
  }
  else if(distancia_direita()>=10 && distancia_direita()>distancia_esquerda()){
    gira_direita();
    Serial.println("girando direita");
    Serial.println(distancia_direita());
  }
  else if(distancia_esquerda()>=10 && distancia_esquerda()>distancia_direita()){
    gira_esquerda();
    Serial.println("Andando esquerda");
    Serial.println(distancia_esquerda());
  }
  else{
    para();
  Serial.println("parei");
  }
}

float distancia_frente()
{  
  // apenas para limpar o pino transmissor, cortar o sinal e aguardar 5us segundos  
  // (recomendado p/ melhor funcionamento) 
  digitalWrite(pinoTransmissor, LOW);
  delayMicroseconds(5); 
  digitalWrite(pinoTransmissor, HIGH); 
  delayMicroseconds(10);
  digitalWrite(pinoTransmissor, LOW);
  int duracao = pulseIn(pinoReceptor, HIGH);
  float calcDistancia= (duracao/2) * 0.0343; 
  if (calcDistancia>=331){
      calcDistancia=0;
  }
  return calcDistancia;  
}

float distancia_direita()
{  
  // apenas para limpar o pino transmissor, cortar o sinal e aguardar 5us segundos  
  // (recomendado p/ melhor funcionamento) 
  digitalWrite(pinoTransmissorDireita, LOW);
  delayMicroseconds(5); 
  digitalWrite(pinoTransmissorDireita, HIGH); 
  delayMicroseconds(10);
  digitalWrite(pinoTransmissorDireita, LOW);
  int duracao = pulseIn(pinoReceptorDireita, HIGH);
  float calcDistancia= (duracao/2) * 0.0343; 
  if (calcDistancia>=331){
      calcDistancia=0;
  }
  return calcDistancia;  
}

float distancia_esquerda()
{  
  // apenas para limpar o pino transmissor, cortar o sinal e aguardar 5us segundos  
  // (recomendado p/ melhor funcionamento) 
  digitalWrite(pinoTransmissorEsquerda, LOW);
  delayMicroseconds(5); 
  digitalWrite(pinoTransmissorEsquerda, HIGH); 
  delayMicroseconds(10);
  digitalWrite(pinoTransmissorEsquerda, LOW);
  int duracao = pulseIn(pinoReceptorEsquerda, HIGH);
  float calcDistancia= (duracao/2) * 0.0343; 
  if (calcDistancia>=331){
      calcDistancia=0;
  }
  return calcDistancia;  
}

int detecta_cor(){
  
  //Vermelho
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  valorVermelho = pulseIn(Out, digitalRead(Out) == HIGH ? LOW : HIGH);
  // Serial.println("Lendo Vermelho");
  // Serial.println(valorVermelho);
  
  //Sem filtro
  digitalWrite(S2, HIGH);
  valorBranco = pulseIn(Out, digitalRead(Out) == HIGH ? LOW : HIGH);
  // Serial.println("Lendo Branco");
  // Serial.println(valorBranco);

  //Azul
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  valorAzul = pulseIn(Out, digitalRead(Out) == HIGH ? LOW : HIGH);
  // Serial.println("Lendo Azul");
  // Serial.println(valorAzul);

  //Verde
  digitalWrite(S2, HIGH);
  valorVerde = pulseIn(Out, digitalRead(Out) == HIGH ? LOW : HIGH);
  // Serial.println("Lendo Verde");
  // Serial.println(valorVerde);
  
    // Analisa se a menor frequencia é amarelo e liga o led amrelo
  if (valorVermelho < valorVerde && valorVermelho < valorAzul && valorBranco < 150 && valorAzul > valorVerde) {
    Serial.println("Cor detectada: Amarelo");
    Serial.println("Indo para reciclagem de metal");    
    digitalWrite(R, LOW);
    digitalWrite(G, LOW);
    digitalWrite(B, LOW);
    digitalWrite(Y, HIGH);
    return 3;
  }

  // Analisa se a menor frequencia é vermelha e liga o led vermelho
  else if (valorVermelho < valorVerde && valorVermelho < valorAzul && valorBranco < 150) {
    Serial.println("Cor detectada: Vermelho");
    Serial.println("Indo para reciclagem de plastico");
    digitalWrite(R, HIGH);
    digitalWrite(G, LOW);
    digitalWrite(B, LOW);
    digitalWrite(Y, LOW);
    return 1;
  }
  
  // Analisa se a menor frequencia é verde e liga o led verde
  else if (valorVerde < valorVermelho && valorVerde < valorAzul && valorBranco < 150) {
    Serial.println("Cor detectada: Verde");
    Serial.println("Indo para reciclagem de vidro");   
    digitalWrite(R, LOW);
    digitalWrite(G, HIGH);
    digitalWrite(B, LOW);
    digitalWrite(Y, LOW);
    return 4;
  }
  
  // Analisa se a menor frequencia é azul e liga o led azul
  else if (valorAzul < valorVermelho && valorAzul < valorVerde  && valorBranco < 150) {
    Serial.println("Cor detectada: Azul"); 
    Serial.println("Indo para reciclagem de papel");   
    digitalWrite(R, LOW);
    digitalWrite(G, LOW);
    digitalWrite(B, HIGH);
    digitalWrite(Y, LOW);
    return 2;
  }
  
  delay(1000);  // Aguarda 1 segundo e reinicia leitura
}

void abre_portao(){
  while(posicao_servo1 <=90 && posicao_servo2 >=90){
    posicao_servo1++;
    posicao_servo2--;
    servo_1.write(posicao_servo1);
    servo_2.write(posicao_servo2);
  }
}

void fecha_portao(){
  while(posicao_servo1 >=0 && posicao_servo2 <=180){
    posicao_servo1--;
    posicao_servo2++;
    servo_1.write(posicao_servo1);
    servo_2.write(posicao_servo2);
  }
}
