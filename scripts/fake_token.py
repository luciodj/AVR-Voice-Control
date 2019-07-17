from flask import jsonify

def fake_token(request):
    """Responds to an authorization token request 
    Args:
        request (flask.Request): HTTP request object.
    Returns:
        The response text or any set of values that can be turned into a
        Response object using
        `make_response <http://flask.pocoo.org/docs/1.0/api/#flask.Flask.make_response>`.
    """
    grant_type = request.form['grant_type']
    obj = {}
    if grant_type == 'authorization_code':
        obj = {
            'token_type': 'bearer',
            'access_token': '123access',
            'refresh_token': '123refresh',
            'expires_in' : 86400  #seconds in a day
        }
    elif grant_type == b'refresh_token':
        obj = {
            'token_type': 'bearer',
            'access_token': '123access',
            'expires_in' : 86400  #seconds in a day
        }
                
    else:
        print('Ukn Grant_type:', grant_type)        
        
    return jsonify(obj) 