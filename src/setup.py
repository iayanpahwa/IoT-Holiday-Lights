from flask import Flask, render_template, request

app = Flask(__name__)

# Load the main form template on webrequest for the root page
@app.route("/")
def main():

    # Create a template data dictionary to send any data to the template
    templateData = {
        'title' : 'IoT Holiday Lights'
        }
    # Pass the template data into the template powrcycle.html and return it to the user
    return render_template('index.html', **templateData)

@app.route("/onn/<state>")
def action_onn(state):
        if state == 'A':
			return "Style A on lights activated\n"

        elif state == 'B':
            return "Style B on lights activated\n"


@app.route("/off/<state>")
def action_off(state):
        if state == 'A':
            return "Style A on lights deactivated\n"

        elif state == 'B':
			return "Style B on lights deactivated\n\n"

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=80, debug=True)
