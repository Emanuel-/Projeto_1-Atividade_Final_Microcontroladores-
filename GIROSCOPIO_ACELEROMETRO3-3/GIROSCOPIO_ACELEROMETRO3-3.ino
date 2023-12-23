#include<Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MPU6050_light.h>
MPU6050 mpu(Wire);
unsigned long timer = 0;


//Endereço em hexadecimal do sensor MPU 6050
const int ENDERECO_SENSOR=0x68;
unsigned long time=millis(), time_enter=millis(),time_repouso=millis();
char caracter_morse;
bool terminou_conversao;
bool retornando_repouso;
bool apertou_enter=false; 

int girX, girY, girZ, acelX, acelY, acelZ, temperatura;
bool comandoCima=false,comandoBaixo=false;
float accX, accY, accZ;
float angle1, angle2;
float elapsedTime, currentTime, previousTime;
float yaw;
bool convertendo=false;

int quantcliques=0;
char morse_code(int girZ)
{
  if(quantcliques==0 && girZ/131>100&&millis()-time>1200&&millis()-time_enter>1000)
  {
    time=millis();
    apertou_enter=false;
    quantcliques++;
    terminou_conversao=false;
    convertendo=true;
    retornando_repouso=true;
    time_repouso=millis();
  } else {
    if(millis()-time<2000)
    {
      if(millis()-time>200)
      {
        if(quantcliques==1&&girZ/131>100)
        {
          
          quantcliques=0;
          //Serial.println("-");
          terminou_conversao=true;
          retornando_repouso=true;
          time_repouso=millis();
          return '1';
        }
      }
      
    }
    else
    {
      if(quantcliques==1)
      {
        
        quantcliques=0;
        //Serial.println(".");
        //Mouse.click();
        terminou_conversao=true;
        return '0';
      }
    }
  }
    
}
void apertar_enter(int girY)
{
  if(girZ/131<-100&&retornando_repouso==false&&apertou_enter==false)
  {
    //Serial.println(retornando_repouso);
    Serial.println("2");
    apertou_enter=true;
    time_enter=millis();
    delay(2000);
  }
    
}








void setup()
{
  Serial.begin(9600);

  //Inicializa a biblioteca Wire
  Wire.begin();
  Wire.beginTransmission(ENDERECO_SENSOR);
  Wire.write(0x6B); 
   
  //Inicializa o sensor
  Wire.write(0); 
  Wire.endTransmission(true);

  mpu.begin();
  mpu.calcGyroOffsets();  


  
}
void loop()
{

  
  
  //Começa uma transmissão com o sensor
  Wire.beginTransmission(ENDERECO_SENSOR);

  //Enfilera os bytes a ser transmitidos para o sensor
  //Começando com o registor 0x3B
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)

  //Finaliza e transmite os dados para o sensor. O false fará com que seja enviado uma mensagem 
  //de restart e o barramento não será liberado
  Wire.endTransmission(false);
  
  //Solicita os dados do sensor, solicitando 14 bytes, o true fará com que o barramento seja liberado após a solicitação 
  //(o valor padrão deste parâmetro é true)
  Wire.requestFrom(ENDERECO_SENSOR, 14, true);  
  previousTime = currentTime;        // Previous time is stored before the actual time read
  currentTime = millis();            // Current time actual time read
  elapsedTime = (currentTime - previousTime) / 1000; // Divide by 1000 to get seconds
  
  

  
  
  //Armazena o valor dos sensores nas variaveis correspondentes
  acelX = Wire.read()<<8|Wire.read();  //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)  
  acelY = Wire.read()<<8|Wire.read();  //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)  
  acelZ = Wire.read()<<8|Wire.read();  //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)  

  accX = (float)acelX/16384;
  accY = (float)acelY/16384;
  accZ = (float)acelZ/16384;

  angle1 = atan(accX/sqrt(accY*accY+accZ*accZ))*1/(3.142/180);
  angle2 = atan(accY/sqrt(accX*accX+accZ*accZ))*1/(3.142/180);
 
  temperatura = Wire.read()<<8|Wire.read();  //0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)

  girX = Wire.read()<<8|Wire.read();  //0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)     
  girY = Wire.read()<<8|Wire.read();  //0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  girZ = Wire.read()<<8|Wire.read();  //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
 caracter_morse=morse_code(girZ);
  //Serial.println(terminou_conversao);
  if(retornando_repouso&&millis()-time_repouso>1500)
  {
    retornando_repouso=false;
    //Serial.println("Entrei aqui");
    
  }

  if(terminou_conversao)
  {
    Serial.println(caracter_morse);
    terminou_conversao=false;
    convertendo=false;
  }
  apertar_enter(girY);

 
  //Serial.println(yaw);

  mpu.update();  
 
  // Cálculo da velocidade angular em torno do eixo Z
  float velocidadeAngularZ = (float)girZ / 131.0; // Valor bruto do giroscópio convertido para graus por segundo
                           
  //Serial.println("Yaw");
  //Serial.println(mpu.getAngleZ());
  if(convertendo==false)
  {
      if(mpu.getAngleZ()>10)
    {
      
      //maximo_anguloZ=angleMax();
      Serial.print("Esquerda\t");
      Serial.print(-mpu.getAngleZ());
      Serial.print("\tVelocidade Angular Z: ");
      Serial.println(velocidadeAngularZ);
      
    }
    else{
      if(mpu.getAngleZ()<-10)
      {
        Serial.print("Direita\t");
        Serial.print(-mpu.getAngleZ());
        Serial.print("\tVelocidade Angular Z: ");
        Serial.println(velocidadeAngularZ);
      }
    }
  }
  
  //Serial.println(angle1);
  if(angle1>10)
  {
    Serial.print("Baixo\t");
    Serial.print(-angle1);
    Serial.print("\tVelocidade Angular Z: ");
    Serial.println(velocidadeAngularZ);
  }
  else
  {
    if(angle1<-20)
    {
      Serial.print("Cima\t");
      Serial.print(-angle1);
      Serial.print("\tVelocidade Angular Z: ");
      Serial.println(velocidadeAngularZ);
    }
  }
  if(mpu.getAccAngleX()>10)
  {
    Serial.print("Esquerda eixo x\t");
    Serial.print(-mpu.getAccAngleX());
    Serial.print("\tVelocidade Angular Z: ");
    Serial.println(velocidadeAngularZ);
  }
  else
  {
    if(mpu.getAccAngleX()<-20)
    {
      Serial.print("Direita eixo x\t");
      Serial.print(-mpu.getAccAngleX());
      Serial.print("\tVelocidade Angular Z: ");
      Serial.println(velocidadeAngularZ);
    }
  }
  delay(500);
  
  
}
