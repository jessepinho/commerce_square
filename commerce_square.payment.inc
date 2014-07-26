<?php

/**
 * @file
 * Payment method callbacks for the Commerce Square Payment Method module.
 */

/**
 * Payment method callback: submit form.
 */
function commerce_square_submit_form($payment_method, $pane_values, $checkout_pane, $order) {
  $path = drupal_get_path('module', 'commerce_square') . '/images';
  $description = theme('item_list', array('type' => 'ol', 'items' => array(
    theme('image', array('path' => "$path/square-gear.jpg")),
    theme('image', array('path' => "$path/square-recent-activity.jpg")),
    theme('image', array('path' => "$path/square-receipt-number.jpg")),
  )));
  $form['remote_id'] = array(
    '#type' => 'textfield',
    '#title' => t("Receipt number"),
    '#description' => t('In the iPhone app, this can be found by tapping the gear icon in the top left corner (1), then choosing the payment under "Recent Activity" (2). The receipt number will be displayed under "Details" on the next screen (3). Leave blank if unavailable. !images', array('!images' => $description)),
  );
  $form['status'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#options' => array(
      COMMERCE_PAYMENT_STATUS_SUCCESS => t('Success'),
      COMMERCE_PAYMENT_STATUS_FAILURE => t('Failure'),
      COMMERCE_PAYMENT_STATUS_PENDING => t('Pending'),
    ),
    '#default_value' => COMMERCE_PAYMENT_STATUS_SUCCESS,
  );

  return $form;
}

/**
 * Payment method callback: submit form submission.
 */
function commerce_square_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $charge) {
  $transaction = commerce_payment_transaction_new('square', $order->order_id);
  $transaction->instance_id = $payment_method['instance_id'];
  $transaction->amount = $charge['amount'];
  $transaction->currency_code = $charge['currency_code'];
  $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;

  if (!empty($pane_values['remote_id'])) {
    $transaction->remote_id = $pane_values['remote_id'];
  }
  commerce_payment_transaction_save($transaction);
}
