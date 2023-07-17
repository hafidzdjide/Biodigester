/***************************************************
* Liquid Level Sensor-XKC-Y25-T12V
* ****************************************************
* This example is to get liquid level

* @author jackli(Jack.li@dfrobot.com)
* @version  V1.0
* @date  2016-1-30

* GNU Lesser General Public License.
* See <http://www.gnu.org/licenses/> for details.
* All above must be included in any redistribution
* ****************************************************/
int Liquid_level=0;
void setup() {
 Serial.begin(9600);
 pinMode(7,INPUT);
 pinMode(4, OUTPUT);
}

void loop() {
Liquid_level=digitalRead(7);
Serial.print("Liquid_level= ");

Serial.println(Liquid_level,DEC);
if (Liquid_level == 1)
{
    Serial.print ("Air Penuh");
    digitalWrite (4, HIGH);
}
else
{
    Serial.print ("Air Kosong");
    digitalWrite (4, LOW);
}

delay(500);
}
