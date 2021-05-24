import boto3

def detect_labels(bucket, label):

    client=boto3.client('rekognition','us-east-1')

    s3= boto3.client('s3')
    files= s3.list_objects(Bucket= bucket)['Contents']

    for f in files:
        response = client.detect_labels(Image={'S3Object':{'Bucket':bucket,'Name':f['Key']}})
  
        for l in response['Labels']:
            if l['Name'] == label and l['Confidence'] > 70:
                print(f['Key'] + " Confianza: " + str(l['Confidence']))

def main():
    bucket="awspruebaia"
    print("Introduzca su label: ", end="")
    detect_labels(bucket, input())

if __name__ == "__main__":
    main()


