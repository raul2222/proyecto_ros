import boto3

bucket="awspruebaia"
collectionId="Amigos"
fileName='parejo4.png'#Modificar para ver si está en nuestra colección
threshold = 1
maxFaces=3

client=boto3.client("rekognition","us-west-2")
  
response=client.search_faces_by_image(CollectionId=collectionId,
                            Image={'S3Object':{'Bucket':bucket,'Name':fileName}},
                            FaceMatchThreshold=threshold,
                            MaxFaces=maxFaces)

                                
faceMatches=response['FaceMatches']
print ('Matching faces')
for match in faceMatches:
    print ('FaceId:'  + match['Face']['ExternalImageId'])
    print ('Similarity: ' + "{:.2f}".format(match['Similarity']) + "%")
    print