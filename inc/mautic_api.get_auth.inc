<?php

use Mautic\Auth\ApiAuth;

/**
 * This will make the call to get authenticated with a Mautic install. 
 * 
 * It will return the $auth object to be used elsewhere for API calls. 
 * 
 * See https://packagist.org/packages/mautic/api-library. This is mostly copied from there. 
 * 
 * Other than the use of variable_get and variable_set
 * 
 * @global type $base_url
 * @return type
 */

function mautic_api_get_authenticated () {
  
  global $base_url;
  $mautic_base_url = variable_get('mautic_api_base_url');
  $accessTokenData = array(
    'accessToken' => variable_get('mautic_api_access_token'),
    'accessTokenType' => variable_get('mautic_api_access_token_type'),
    'accessTokenExpires' => variable_get('mautic_api_access_token_expires'),
    'accessTokenRefresh' => variable_get('mautic_api_access_token_refresh'),
  );
  // ApiAuth->newAuth() will accept an array of Auth settings
  $settings = array(
    'clientKey' => variable_get('mautic_api_client_id'),
    'clientSecret' => variable_get('mautic_api_client_secret'),
    'callback' => $base_url . '/admin/config/system/mautic-api', // @todo Change this to your app callback. It should be the same as you entered when you were creating Mautic API credentials.
    'version' => 'OAuth2',
    'baseUrl' => $mautic_base_url,
  );
  
  // If you already have the access token, et al, pass them in as well to prevent the need for reauthorization
  if (!empty($accessTokenData['accessToken']) && !empty($accessTokenData['accessTokenExpires'])) {
    $settings['accessToken'] = $accessTokenData['accessToken'] ;
    $settings['accessTokenType'] = $accessTokenData['accessTokenSecret'];
    $settings['accessTokenExpires'] = $accessTokenData['accessTokenExpires'];
    $settings['accessTokenRefresh'] = $accessTokenData['accessTokenExpires'];
  }

  // Initiate the auth object
  $initAuth = new ApiAuth();
  $auth = $initAuth->newAuth($settings);

  // Initiate process for obtaining an access token; this will redirect the user to the $authorizationUrl and/or
  // set the access_tokens when the user is redirected back after granting authorization

  // If the access token is expired, and a refresh token is set above, then a new access token will be requested
  try {
    if ($auth->validateAccessToken()) {

      // Obtain the access token returned; call accessTokenUpdated() to catch if the token was updated via a
      // refresh token

      // $accessTokenData will have the following keys:
      // access_token, expires, token_type, refresh_token

      if ($auth->accessTokenUpdated()) {
        $accessTokenData = $auth->getAccessTokenData();
        variable_set('mautic_api_access_token', $accessTokenData['access_token']);
        variable_set('mautic_api_access_token_expires', $accessTokenData['expires']);
        variable_set('mautic_api_access_token_refresh', $accessTokenData['refresh_token']);
        variable_set('mautic_api_access_token_type', $accessTokenData['token_type']);
      }
    }
  } catch (Exception $e) {
    // Do Error handling
    $message = "There was the following error returned when trying to authenticate:<pre>";
    $message .= $e;
    $message .= "</pre>";
    watchdog('mautic_api', $message);
  }

  return $auth;
}