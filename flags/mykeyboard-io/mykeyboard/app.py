from flask import Flask, render_template, send_from_directory, request, jsonify
from flask.ext.sqlalchemy import SQLAlchemy

app = Flask(__name__)
app.config.from_pyfile('app.cfg')

db = SQLAlchemy(app)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/<path:resource>')
def serveStaticResource(resource):
    return send_from_directory('static/', resource)

@app.route('/signup', methods=['POST'])
def signup():
    name = request.form['name']
    email = request.form['email']
    code = request.form['code']

    try:
        res = db.engine.execute("select code from invite where code = '{}'".format(code));
        if len(list(res)) < 1:
            return jsonify({
                'success': False,
                'error': 'Invalid invite code'
            })

        res = db.engine.execute("select name, email from signup where email = '{}'".format(email))
        res = list(res)
        if len(res) > 0:
            return jsonify({
                'success': False,
                'error': 'The email "{}" is already used by {}'.format(res[0][1], res[0][0])
            })

        db.engine.execute("insert into signup(name, email) values ('{}', '{}')".format(name, email))
        db.session.commit()

    except:
        db.session.rollback()
        return jsonify({
            'success': False,
            'error': 'Unknown database error'
        })

    return jsonify({
        'success': True,
    })
