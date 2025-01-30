from flask import Flask, send_from_directory, request
import os

#local image server 

app = Flask(__name__)

IMAGE_FOLDER = 'Image folder path'

@app.route('/image/<filename>')
def get_image(filename):
    return send_from_directory(IMAGE_FOLDER, filename)

@app.route('/upload', methods=['POST'])
def upload_file():
    image = request.files['image']
    image.save(os.path.join(IMAGE_FOLDER, 'memory_image.jpg'))
    return 'Successful image upload', 200

if __name__ == '__main__':
    app.run(host='0.0.0.0',port=5000,debug=True)


# http://127.0.0.1:5000/image/PXL_20231124_205503875.jpg
