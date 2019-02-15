
import boto3

"""
Handy dandy utilities for dealing with AWS resources
"""

def get_parameter(param_name):
    """
    This function reads a secure parameter from AWS' SSM service.
    The request must be passed a valid parameter name, as well as 
    temporary credentials which can be used to access the parameter.
    The parameter's value is returned.
    """
    # Create the SSM Client
    ssm = boto3.client('ssm')

    #check that we have creds here

    # Get the requested parameter
    response = ssm.get_parameters(
        Names=[
            param_name,
        ],
        WithDecryption=True
    )
    
    # Store the credentials in a variable
    credentials = response['Parameters'][0]['Value']

    return credentials