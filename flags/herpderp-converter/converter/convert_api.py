from flask import Blueprint, request, redirect, url_for, send_from_directory, render_template
from werkzeug import secure_filename
import wand.image

import os

convert_api = Blueprint('convert_api', __name__)

@convert_api.record
def record_params(setup_state):
    convert_api.config = setup_state.app.config

@convert_api.route('/convert', methods=['GET', 'POST'])
def create():
    if request.method == 'POST':
        file = request.files['file']
        if not file:
            return render_template('convert_api/error1.html')
        if not allowed_file(file.filename):
            return render_template('convert_api/error2.html')
        filename = secure_filename(file.filename)
        filename = '.'.join(filename.split('.')[:-1]) + '.pdf'
        try:
            with wand.image.Image(file=file.stream) as img:
                with img.convert('pdf') as converted:
                    converted.save(filename=os.path.join(convert_api.config['UPLOAD_FOLDER'], filename))
            return redirect(url_for('convert_api.uploaded_file',
                                    filename=filename))
        except Exception as e:
            return render_template('convert_api/error3.html')
    return render_template('convert_api/index.html')

def allowed_file(filename):
    return ('.' in filename and 
        filename.rsplit('.', 1)[1] in convert_api.config['ALLOWED_EXTENSIONS'])

@convert_api.route('/uploads/<filename>')
def uploaded_file(filename):
    return send_from_directory(convert_api.config['UPLOAD_FOLDER'], filename)
