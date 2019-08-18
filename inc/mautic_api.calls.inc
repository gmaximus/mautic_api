<?php

use Mautic\MauticApi;

/**
 * This will make a call to Mautic and return a array of fields. 
 * See: https://developer.mautic.org/#list-available-fields
 * @return type
 */

function mautic_api_get_field_list_call() {
 
 // Get $auth object
  $auth = mautic_api_get_auth();

 // Make the call 
  $api_url = variable_get('mautic_api_base_url') . '/';
  $api = new MauticApi();
  $contact_api = $api->newApi('contacts', $auth, $api_url);
  $fields = $contact_api->getFieldList();

  return $fields;
}

/**
 * This will make a call to Mautic and get all data related to a field. 
 * See: https://developer.mautic.org/?php#get-field
 * @return type
 */

function mautic_api_get_contact_field_info_call($mautic_field_id) {
  
  // Get $auth object
    $auth = mautic_api_get_auth();

  // Make the call  
    $api_url = variable_get('mautic_api_base_url') . '/';
    $api = new MauticApi();
    $field_api = $api->newApi("contactFields", $auth, $api_url);
    $field = $field_api->get($mautic_field_id);

    return $field;
}

/**
 * This will fetch a Mautic contact by the provided email
 * See: https://developer.mautic.org/?php#list-contacts
 * @return type
 */

function mautic_api_get_contact_call($email) {
  
  // Get the $auth object to make the call.
    $auth = mautic_api_get_auth();
  
  // Limt search to 1 result
    $limit = 1;
  // Order result by Mautic ID. Couldn't see how to give no value for this.
    $order_by = 'id';
  
  // Make the call to Mautic
    $api_url = variable_get('mautic_api_base_url') . '/';
    $api = new MauticApi();
    $contact_api = $api->newApi('contacts', $auth, $api_url);
    $contacts_list = $contact_api->getList($email, 0, $limit, $order_by, 'desc', FALSE, FALSE);
    $contacts = $contacts_list['contacts'];
  
  // There will only be 1 result because of the limit
    foreach ($contacts as $contact) {
      return $contact;
    }
}

/**
 * This will create a new contact in Mautic
 * We need to pass it a array of field values, keyed on the alias from Mautic.
 * 
 * ie 
 * 
 * $data = array(
 *   'firstname' => 'Jim',
 *   'lastname'  => 'Contact',
 *   'email'     => 'jim@his-site.com',
 *   'ipAddress' => $_SERVER['REMOTE_ADDR']
 * );
 * 
 * See https://developer.mautic.org/?php#create-contact
 * 
 * @param type $data
 */
function mautic_api_create_contact_call($data) {
       
  // Get the $auth object to make the call.
   $auth = mautic_api_get_auth();
   
  // Set the contact owner
   $owner_user_id = variable_get('mautic_api_user_id', '');
   $data['owner'] = intval($owner_user_id);
   
  // Make the call
   $api_url = variable_get('mautic_api_base_url') . '/';
   $api = new MauticApi();
   $contact_api = $api->newApi('contacts', $auth, $api_url); 
   $contact = $contact_api->create($data);
}

/**
 * This will edit a exiting contact in Mautic. Pass it the Mautic Contact ID and $data array.
 * 
 * $data = array(
 *   'firstname' => 'Jim',
 *   'lastname'  => 'Contact',
 *   'email'     => 'jim@his-site.com',
 *   'ipAddress' => $_SERVER['REMOTE_ADDR']
 * );
 * 
 * See https://developer.mautic.org/?php#edit-contact
 * 
 * @param type 
 */
function mautic_api_edit_contact_call($id, $data) {
  
  // Set some local variables
    $create_if_not_found = FALSE;
  
  // Ensure the ID provided is a integer and not string
    $contact_id = intval($id);
    
  // Update Last active
    $data['lastActive'] = date('Y-m-d H:m:i');
    
  // Get the $auth object to make the call.
    $auth = mautic_api_get_auth();

  // Make the call
    $api_url = variable_get('mautic_api_base_url') . '/';
    $api = new MauticApi();
    $contact_api = $api->newApi('contacts', $auth, $api_url); 
    $contact = $contact_api->edit($contact_id, $data, $create_if_not_found);
}