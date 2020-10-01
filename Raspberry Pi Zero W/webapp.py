from gpiozero import LED
from flask import *
app = Flask(__name__)

LED_Relay = LED(21) 
Buzzer = LED(20)
Buzzer.on()

@app.route("/")
def index():
    return render_template('index.html') 
@app.route("/led/<int:state>", methods=['POST'])
def led(state):
    if state == 0:
        LED_Relay.off()
        Buzzer.on()
    elif state == 1:
        LED_Relay.on()
        Buzzer.off()
    else:
        return("Error Unknown State")
    return('',204)

if __name__ == "__main__":
    app.run(host='0.0.0.0', debug=True)
   
