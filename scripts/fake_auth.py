from urllib.parse import parse_qs
from flask import redirect

def fake_auth(request):
    """Responds to an authorization request with a redirect, clientID and state
    Args:
        request (flask.Request): HTTP request object.
    Returns:
        The response text or any set of values that can be turned into a
        Response object using
        `make_response <http://flask.pocoo.org/docs/1.0/api/#flask.Flask.make_response>`.
    """
    query = parse_qs(request.query_string)
 #   print('Query string=', request.query_string)
 #   print('Query=', query)
    redirect_uri = query[b'redirect_uri'][0]
    state = query[b'state'][0]
    response_url =  redirect_uri + b'?code=AVR&state=' + state       

    print('Response url:', response_url)
    return redirect(response_url)
