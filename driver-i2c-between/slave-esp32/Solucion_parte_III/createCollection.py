import boto3
import json

client=boto3.client("rekognition","us-east-1")

def add_faces_to_collection(bucket,filename,facename,collection_id):

    response=client.index_faces(CollectionId=collection_id,
                                Image={'S3Object':{'Bucket':bucket,'Name':filename}},
                                ExternalImageId=facename,
                                MaxFaces=1)

    print (filename + ' added')
    print ('Results for ' + filename) 	
    print('Faces indexed:')						
    for faceRecord in response['FaceRecords']:
         print('  Face ID: ' + faceRecord['Face']['FaceId'])
         print('  Location: {}'.format(faceRecord['Face']['BoundingBox']))

    return len(response['FaceRecords'])

def main():
    bucket='awspruebaia'
    collection_id='Amigos'

    client.delete_collection(CollectionId=collection_id)
    client.create_collection(CollectionId=collection_id)
        
    indexed_faces_count=add_faces_to_collection(bucket, 'parejo1.png', 'parejo', collection_id) #Aquí modificar para añadir tus amigos
    indexed_faces_count=add_faces_to_collection(bucket, 'gaya1.png', 'gaya', collection_id)
    indexed_faces_count=add_faces_to_collection(bucket, 'guedes1.jpg', 'guedes', collection_id)

    print("Faces indexed count: " + str(indexed_faces_count))

if __name__ == "__main__":
    main()

   
