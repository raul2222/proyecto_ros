import boto3
import json

def get_text(image, bucket, p_ofensivas):

    client=boto3.client('rekognition','us-east-1')

    response=client.detect_text(Image={'S3Object':{'Bucket':bucket,'Name':image}})
                        
    textDetections=response['TextDetections']
    for t in response['TextDetections']:
       if t['DetectedText'] in p_ofensivas or t['DetectedText'][:-1] in p_ofensivas:
           print(t['DetectedText'])
    
    return len(textDetections)
    
def main():

    bucket='awspruebaia'
    image='Imagen_ofensiva.jpeg'
    p_ofensivas= ["Estupido","ignorante","majadero","traidor","odias","terroristas"]
    text_count=get_text(image,bucket)

    print("Text detected: " + str(text_count))


if __name__ == "__main__":
    main()    