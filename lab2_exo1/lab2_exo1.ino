#define INTERVAL 200

const int gasSensorPin = A1;                // Pin du capteur (A pour un pin analogique)

int gasSensorVal = 0;                       // D'eclaration de la valeur du capteur de gaz

void setup() {
 Serial.begin(9600);                        // D'emmarage de l'interface d'affichade avec un baudrate de 9600
}

void loop() {
  gasSensorVal = analogRead(gasSensorPin);  // On lit la valeur du capteur de gaz
  Serial.println(gasSensorVal);             // On l'affiche sur la sortie

  delay(INTERVAL);                           // On attend l'intervalle de temps entre chaque acquisition 
}