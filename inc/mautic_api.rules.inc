<?php

use Mautic\Auth\ApiAuth;
use Mautic\MauticApi;

/**
 * Implements hook_rules_data_info()
 * 
 * This is where we tell Rules about our new "mautic_contact" data type for use in the actions
 * 
 */
function mautic_api_rules_data_info() {
  return array(
    'mautic_contact' => array(
      'label' => t('Mautic contact'),
      'wrap' => TRUE,
      'property info' => mautic_api_get_contact_data_fields(),
    ),
  );
}

/**
 * Implements hook_rules_action_info().
 * 
 */
function mautic_api_rules_action_info() {
  
  // Define the Get contact action
  $items['mautic_api_get_contact'] = array(
    'label' => t('Get Mautic contact.'),
    'base' => 'mautic_api_get_contact_action',
    'group' => t('Mautic API'),
    'module' => 'mautic_api',
    'parameter' => array(
     'user' => array(
       'label' => t('User'), 
       'type' => t('user'),
      ),
    ),
    'provides' => array(
      'mautic_contact' => array(
        'type' => 'mautic_contact',
        'label' => t('Mautic contact'),
      ),
    ),
  );
  
  // Define the Save contact action 
  $items['mautic_api_save_contact'] = array(
   'label' => t('Send the Mautic contact back.'),
   'base' => 'mautic_api_save_contact_action',
   'group' => t('Mautic API'),
   'module' => 'mautic_api',
   'parameter' => array(
     'mautic_contact' => array(
       'label' => t('Mautic contact'), 
       'type' => t('mautic_contact'),
     ),
   ),
 );
  
  return $items;
}

/**
 * The function is called by the get contact action
 * 
 * @param type $user
 * @return $mautic_contact
 */
function mautic_api_get_contact_action($user) {
  
}

/**
 * This function is called by the Save contact action
 * 
 * @param type $mautic_contact
 */
function mautic_api_save_contact_action($mautic_contact) {
  
}

/**
 * This will query Mautic for a contact by a given email address.
 * 
 * @param type $email
 * @param type $mautic_contact
 */
function mautic_api_check_contact_exists($email) {
  
}

/**
 * This will create a new contact in Mautic
 * @param type $user
 */
function mautic_api_create_contact($user) {
  
}

/**
 * This retrieves the fields associated to a contact and builds an array that Rules understands.
 * 
 * @return type
 */
function mautic_api_get_contact_data_fields() {
  
  $rules_fields = array();
  $mautic_fields = mautic_api_get_contact_fields();
  
  foreach ($mautic_fields as $mautic_field) {
    $rules_fields[] = mautic_api_convert_mautic_field_to_drupal($mautic_field);
  }
  
  return $rules_fields;
  
  /** Example array for what needs to be returned
  return array(
    'type' => array(
      'type' => 'text',
      'label' => t('The category to which this message belongs'),
    ),
    'message' => array(
      'type' => 'text',
      'label' => t('Log message'),
      'getter callback' => 'rules_system_log_get_message',
      'sanitized' => TRUE,
    ),
    'severity' => array(
      'type' => 'integer',
      'label' => t('Severity'),
      'options list' => 'watchdog_severity_levels',
    ),
    'request_uri' => array(
      'type' => 'uri',
      'label' => t('Request uri'),
    ),
  );
   *  
   */
}

/**
 * This takes a Mautic field and return an array in Drupal speak.
 * 
 * @param type $mautic_field
 * @return array
 */
function mautic_api_convert_mautic_field_to_drupal($mautic_field) {
  
  $drupal_field = array();
  $mautic_field_type = $mautic_field['type'];
  
  switch ($mautic_field_type) {
    
    case 'text':


      break;

    default:
      break;
  }
   return $drupal_field;
  
}