import boto3
import logging
from botocore.exceptions import ClientError
def searchFace(buckt, pic, collection):
    bucket= buckt
    collectionId=collection
    fileName=pic
    threshold = 1
    maxFaces=1

    client=boto3.client("rekognition","us-east-1")
  
    response = client.detect_labels(Image={'S3Object':{'Bucket':bucket,'Name':fileName}})
    cara = False
    for label in response['Labels']:
        
        if label['Name']== "Face":
            print(label['Name'])
            print(label['Confidence'])
            if label['Confidence']>=75:
                
                cara = True
            else:
                return "None"
    if cara == True:
        response=client.search_faces_by_image(CollectionId=collectionId,
                            Image={'S3Object':{'Bucket':bucket,'Name':fileName}},
                            FaceMatchThreshold=threshold,
                            MaxFaces=maxFaces)

                                
        faceMatches=response['FaceMatches']
        print ('Matching faces')

        for match in faceMatches:
            print ('FaceId:'  + match['Face']['ExternalImageId'])
            print ('Similarity: ' + "{:.2f}".format(match['Similarity']) + "%")
            if float("{:.2f}".format(match['Similarity']))>=99:
                return match['Face']['ExternalImageId']
        return "Intruder"   


def upload_file(file_name, bucket, object_name=None):
    """Upload a file to an S3 bucket

    :param file_name: File to upload
    :param bucket: Bucket to upload to
    :param object_name: S3 object name. If not specified then file_name is used
    :return: True if file was uploaded, else False
    """

    # If S3 object_name was not specified, use file_name
    if object_name is None:
        object_name = file_name

    # Upload the file
    s3_client = boto3.client('s3')
    try:
        response = s3_client.upload_file(file_name, bucket, object_name)
    except ClientError as e:
        logging.error(e)
        return False
    return True 
                
                
        
if __name__ == "__main__": 
    upload_file("noe2.jpg", "rekpractica567", object_name=None)
    cara = searchFace("rekpractica567","noe2.jpg","gente")
    print(cara)
    
    
    

