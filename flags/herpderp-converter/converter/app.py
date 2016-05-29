from flask import Flask, render_template

app = Flask(__name__)
app.config.from_pyfile('app.cfg')

from convert_api import convert_api
app.register_blueprint(convert_api)

@app.route('/')
def index():
    return render_template('index.html')
