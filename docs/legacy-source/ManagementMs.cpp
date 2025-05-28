#include "BlxContentServiceData.h"
#include "ManagementMs.h"

using namespace System::EnterpriseServices;
using namespace System::Threading;

#include "Utilities.h"

CManagementMs::CManagementMs( LPWSTR szConnectionString )
{
	m_ConnStr = Marshal::PtrToStringUni( static_cast<IntPtr>(szConnectionString ) );
        this->m_commandCollection = gcnew cli::array< ::System::Data::IDbCommand^  >( MGT_MAX_COMMAND_ENTRIES );
        this->m_commandCollection[ MGT_LOG_KEEP_ALIVE ] = (gcnew ::System::Data::SqlClient::SqlCommand());
		
	(cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ MGT_LOG_KEEP_ALIVE ]))->CommandText = L"dbo.uspLogKeepAlive";
    (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ MGT_LOG_KEEP_ALIVE ]))->CommandType = ::System::Data::CommandType::StoredProcedure;
    (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ MGT_LOG_KEEP_ALIVE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@RETURN_VALUE", 
            ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::ReturnValue, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
            false, nullptr, L"", L"", L"")));
    (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ MGT_LOG_KEEP_ALIVE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@EventID", 
            ::System::Data::SqlDbType::UniqueIdentifier, 16, ::System::Data::ParameterDirection::Input, 0, 0, nullptr, ::System::Data::DataRowVersion::Current, 
            false, nullptr, L"", L"", L"")));
    (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ MGT_LOG_KEEP_ALIVE ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@ConferenceParticipantID", 
            ::System::Data::SqlDbType::UniqueIdentifier, 16, ::System::Data::ParameterDirection::Input, 0, 0, nullptr, ::System::Data::DataRowVersion::Current, 
            false, nullptr, L"", L"", L"")));

	return;
}

CManagementMs::~CManagementMs(void)
{
// add code to iterate through array of commands and close all and delete objects...
	return;
}


DWORD CManagementMs::LogKeepAlive(
		PIP_HEADER pIpHeader
		, PUDP_HEADER pUdpHeader
		, PEVENT_HEADER pEventHeader
		, PEVENT_PACKET_HEADER pEventPacketHeader
		, PPACKET_DATA_BLOB pPacketDataBlob
		)
{
	::System::Data::SqlClient::SqlCommand^  command;
	::System::Int32 returnValue = 0;

	/*removed for now*/
	//try
	//{

	//	command = 
	//		(cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ MGT_LOG_KEEP_ALIVE ]));

	//	System::Threading::Monitor::Enter( command );

	//	command->Connection = gcnew ::System::Data::SqlClient::SqlConnection( m_ConnStr );

	//	command->Parameters[ L"@EventID" ]->Value = 
	//	Utilities::FromGUID( pEventHeader->eventId );
	//	command->Parameters[ L"@ConferenceParticipantID" ]->Value = 
	//	Utilities::FromGUID( pEventPacketHeader->pptParticipantId );

	//	::System::Data::ConnectionState previousConnectionState = command->Connection->State;
	//	if ((command->Connection->State & ::System::Data::ConnectionState::Open) 
	//				!= ::System::Data::ConnectionState::Open) {
	//		command->Connection->Open();
	//	}

	//	try {

	//		returnValue = command->ExecuteNonQuery();

	//	}
	//	finally 
	//	{
	//		command->Connection->Close();
	//	}
	//}
	//finally
	//{
	//	System::Threading::Monitor::Exit( command );
	//}

	return returnValue;
}