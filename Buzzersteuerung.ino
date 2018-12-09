/* Knipskiste 2
 * DSLR-Kameraansteuerung mittels Buzzer
 * 
 * 09.12.2018
 * Dr. Matthias Offer
 * 
 * Buzzer zwischen Buzzer_PIN und GND
 * 
 * Kameraansteuerung
 * Autofokus zwischen Autofokus_PIN und GND über Optokoppler
 * Ausloeser zwischen Ausloeser_PIN und GND über Optokoppler
 */

// PINs
int Autofokus_PIN= 10;    // setzen des Digital-PINs zur Ansterung des Autofokus
int Ausloeser_PIN= 11;    // setzen des Digital-PINs zur Ansterung des Ausloesers
int Buzzer_PIN= 12;        // setzten des setzen des Digital-PINs zum auslesen des Buzzers

// Zeiten [ms]
int Entprellzeit= 5;    // setzen der Entprellzeit
int Fokusierzeit= 250;  // setzen der Wartezeit für die Fokusierung
int Haltezeit= 100;     // setzen der Ausloeserhaltezeit
int Todzeit= 1800;      // setzen der Todzeit des Ausloesers


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);       // aktivieren der internen LED als Status-LED
  pinMode(Autofokus_PIN, OUTPUT);     // setzt Digital-PIN Autofokus als OUTPUT
  pinMode(Ausloeser_PIN, OUTPUT);     // setzt Digital-PIN Ausloeser als OUTPUT
  pinMode(Buzzer_PIN, INPUT_PULLUP);  // setzt Digital-PIN Buzzer_PIN als IMPUT mit PULLUP
}


int Buzzerentprellung(int Input_PIN) {
  static int Buzzerstatus;
  if (digitalRead(Input_PIN) == LOW) {
    // eine Aenderung am Eingang wird erkannt
    delay(Entprellzeit);
    if (digitalRead(Input_PIN) == LOW) {
      // nach ablauf der Einprellzeit gibt es keine erneute Aenderung am Eingang --> kein Prellen           
      if (Buzzerstatus == HIGH) {
        //eine eindeutige Aenderung am Eingang wurde erfasst und durch die Funktion zurueckgemeldet
        Buzzerstatus = LOW;
        return LOW;
      }
      if (Buzzerstatus == LOW) {
        //setzt den Eingang zurueck
        return HIGH;
      }   
    }
  }
  //Eingang hat geprellt oder war HIGH (Standartzustand)
  Buzzerstatus = HIGH;
  return HIGH;
}


void Kameraansteuerung() {
  digitalWrite(LED_BUILTIN, HIGH);      // aktiviert Status-LED
  
  digitalWrite(Autofokus_PIN, HIGH);    // aktiviert Autofokus
  delay(Fokusierzeit);                  // wartet auf Fokusierung
  
  digitalWrite(Ausloeser_PIN, HIGH);    // triggert Ausloeser
  delay(Haltezeit);                     // haelt Ausloeser
  
  digitalWrite(Ausloeser_PIN, LOW);     // deaktiviert Ausloeser
  digitalWrite(Autofokus_PIN, LOW);     // deaktiviert Autofokus
  
  digitalWrite(LED_BUILTIN, LOW);       // deaktiviert Status-LED
    
  delay(Todzeit);                       // startet Todzeit des Ausloesers
    
  for (int i=0; i <= 3; i++){         
    // Status-LED blinkt wenn Todzeit abgelaufen ist
    digitalWrite(LED_BUILTIN, HIGH);  // aktiviert Status-LED
      delay(40);
    digitalWrite(LED_BUILTIN, LOW);   // deaktiviert Status-LED
      delay(40);
  }
}


void loop() {
  if (Buzzerentprellung(Buzzer_PIN) == LOW){
    // steuert die Kamera an wenn der Buzzer gedrückt wurde
    // und dadurch das Potential am Buzzer-Eingang auf 0V gelegt wurde
    Kameraansteuerung();
  }
}
