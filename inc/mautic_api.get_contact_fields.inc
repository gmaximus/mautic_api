<?php

use Mautic\MauticApi;

function mautic_api_get_field_list() {
  
  $auth = mautic_api_get_auth();
  $api_url = variable_get('mautic_api_base_url') . '/';

  $api = new MauticApi();
  $contact_api = $api->newApi('contacts', $auth, $api_url);

  $fields = $contact_api->getFieldList();

  return $fields;
}
