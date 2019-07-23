cmake_minimum_required(VERSION 3.5)
project(wxWidget)

find_package(wxWidgets REQUIRED gl core base OPTIONAL_COMPONENTS net)
include(${wxWidgets_USE_FILE})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../)
add_compile_options("-std=c++11")
add_compile_options("-enable-exceptions")
add_definitions(-D WX_STRING)

#
set(UI_SRCS 
    ./UI/BalanceInquiryForm.cc
    ./UI/ButtonPanel.cc
    ./UI/ChangePasswordForm.cc
    ./UI/CloseAccountForm.cc
    ./UI/DepositForm.cc
    ./UI/DetailStatementForm.cc
    ./UI/FormManager.cc
    ./UI/GridPanel.cc
    ./UI/LoginForm.cc
    ./UI/MyWxTools.cc
    ./UI/MyFrame.cc
    ./UI/MainMenuForm.cc
    ./UI/OpenAccountForm.cc
    ./UI/QueryForm.cc
    ./UI/QueryDayBillForm.cc
    ./UI/QueryHistoryBillForm.cc
    ./UI/QueryAccountHistoryBillForm.cc
    ./UI/ReceiptForm.cc
    ./UI/TransferForm.cc
    ./UI/TitlePanel.cc
    ./UI/Validator.cc
    ./UI/WithdrawForm.cc
    ./UI/TransReport.cc
    )

set(PUBLIC_SRCS
  ../public/Exception.cc
  ../public/PackStream.cc
  ../public/Socket.cc
  ../public/StringUtil.cc
  ../public/UnpackStream.cc
  ../public/Logging.cc
  ../public/LogStream.cc
  ../public/Timestamp.cc
  ../public/Encrypt.cc
  ../public/Idea.cc
  ../public/MD5.cc
  )

set(CMD_SRCS
  ./CMD/UserLogin.cc
  ./CMD/OpenAccount.cc
  ./CMD/Deposit.cc
  ./CMD/Withdrawal.cc
  ./CMD/Transfer.cc
  ./CMD/BalanceInquiry.cc
  ./CMD/ChangePassword.cc
  ./CMD/QueryDayBill.cc
  ./CMD/QueryHistoryBill.cc
  ./CMD/QueryAccountHistoryBill.cc
  ./CMD/CloseAccount.cc
  )
set(CLIENT_SRCS
  ./client.cc
  ./BankSession.cc
  ./TransactionManager.cc
  )

add_executable(main ${CLIENT_SRCS} ${UI_SRCS} ${PUBLIC_SRCS} ${CMD_SRCS})
target_link_libraries(main ${wxWidgets_LIBRARIES})

