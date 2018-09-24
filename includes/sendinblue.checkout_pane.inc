
<?php

/**
 * @file
 * Sendinblue checkout panes.
 *
 * Expose a checkbox to subscribe to a newsletter.
 */

/**
 * Payment pane: form callback.
 */
function sendinblue_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {
  global $user;
  $checkout_form= array();

  $response =  SendinblueManager::validationEmail($user->mail, variable_get('sendinblue_pane_newsletter', ''));
  if ($response['code'] != 'already_exist') {
    $checkout_form['sendinblue_pane_checkbox'] = array(
      '#type' => 'checkbox',
      '#title' => check_plain(t('I subscribe to the newsletter to be informed of the latest news')),
      '#attributes' => array('style' => 'width:30px;'),
      '#return_value' => 1,
      '#default_value' => 0,
    );
  }
  return $checkout_form;
}


/**
 * Subscribe the user
 */
function sendinblue_pane_checkout_form_validate($form, &$form_state, $checkout_pane, $order) {
  if ($form_state['values']['sendinblue_pane']['sendinblue_pane_checkbox'] == 1) {
    global $user;
    $response_code = SendinblueManager::subscribeUser($user->mail, array(), array(variable_get('sendinblue_pane_newsletter', '')));
    _addSubscriberTable($email, $info);
  }
  return TRUE;
}

/**
 * Checkout pane callback: returns the cart contents pane's settings form.
 */
function sendinblue_pane_settings_form($checkout_pane) {
  $sendinblue_lists = SendinblueManager::getLists();
  $options = array();
  foreach ($sendinblue_lists as $mc_list) {
    $options[$mc_list['id']] = $mc_list['name'];
  }
  $form['sendinblue_pane_newsletter'] = array(
    '#type' => 'select',
    '#title' => t('List where subscribers are saved'),
    '#options' => $options,
    '#default_value' => variable_get('sendinblue_pane_newsletter', ''),
    '#description' => t('Select the list where you want to add your new subscribers'),
    '#attributes' => array('style' => 'width:200px;'),
  );

  return $form;
}
