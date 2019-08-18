<?php

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
        'type' => 'mautic_api_contact',
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
       'type' => t('mautic_api_contact'),
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
