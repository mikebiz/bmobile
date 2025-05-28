#include "BlxContentServiceData.h"
#include "PublishingPointDestinationMs.h"

using namespace System::EnterpriseServices;
using namespace System::Threading;

#include "Utilities.h"

CPublishingPointDestinationMs::CPublishingPointDestinationMs( LPWSTR szConnectionString )
{
	m_ConnStr = Marshal::PtrToStringUni( static_cast<IntPtr>(szConnectionString ) );
        this->m_commandCollection = gcnew cli::array< ::System::Data::IDbCommand^  >( PPS_MAX_COMMAND_ENTRIES );
        this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SUBSCRIPTION ] = (gcnew ::System::Data::SqlClient::SqlCommand());

		(cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SUBSCRIPTION ]))->CommandText = L"dbo.uspPublishingPointDestinationEstablish";
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SUBSCRIPTION ]))->CommandType = ::System::Data::CommandType::StoredProcedure;
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SUBSCRIPTION ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@RETURN_VALUE", 
                ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::ReturnValue, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SUBSCRIPTION ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@ConferenceID", 
                ::System::Data::SqlDbType::UniqueIdentifier, 16, ::System::Data::ParameterDirection::Input, 0, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SUBSCRIPTION ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@ConferenceParticipantID", 
                ::System::Data::SqlDbType::UniqueIdentifier, 16, ::System::Data::ParameterDirection::Input, 0, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SUBSCRIPTION ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@ipVerLen", 
                ::System::Data::SqlDbType::SmallInt, 2, ::System::Data::ParameterDirection::Input, 5, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SUBSCRIPTION ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@ipTos", 
                ::System::Data::SqlDbType::SmallInt, 2, ::System::Data::ParameterDirection::Input, 5, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SUBSCRIPTION ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@ipTotalLength", 
                ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::Input, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SUBSCRIPTION ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@ipId", 
                ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::Input, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SUBSCRIPTION ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@ipOffset", 
                ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::Input, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SUBSCRIPTION ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@ipTtl", 
                ::System::Data::SqlDbType::SmallInt, 2, ::System::Data::ParameterDirection::Input, 5, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SUBSCRIPTION ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@ipProtocol", 
                ::System::Data::SqlDbType::SmallInt, 2, ::System::Data::ParameterDirection::Input, 5, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SUBSCRIPTION ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@srcIP", 
                ::System::Data::SqlDbType::BigInt, 8, ::System::Data::ParameterDirection::Input, 19, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SUBSCRIPTION ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@destIP", 
                ::System::Data::SqlDbType::BigInt, 8, ::System::Data::ParameterDirection::Input, 19, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SUBSCRIPTION ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@srcPort", 
                ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::Input, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SUBSCRIPTION ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@destPort", 
                ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::Input, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SUBSCRIPTION ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@udpLength", 
                ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::Input, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SUBSCRIPTION ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@MacAddress", 
                ::System::Data::SqlDbType::Char, MACADDRESS_LENGTH, ::System::Data::ParameterDirection::Input, 0, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));

		this->m_commandCollection[ PPS_READ_PPT_VIDEO_SUBSCRIPTIONS ] = (gcnew ::System::Data::SqlClient::SqlCommand());
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_READ_PPT_VIDEO_SUBSCRIPTIONS ]))->CommandText = L"dbo.uspPublishingPointDestinationReadById";
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_READ_PPT_VIDEO_SUBSCRIPTIONS ]))->CommandType = ::System::Data::CommandType::StoredProcedure;
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_READ_PPT_VIDEO_SUBSCRIPTIONS ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@RETURN_VALUE", 
                ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::ReturnValue, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_READ_PPT_VIDEO_SUBSCRIPTIONS ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@PublishingPointId", 
                ::System::Data::SqlDbType::UniqueIdentifier, 16, ::System::Data::ParameterDirection::Input, 0, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));

        this->m_commandCollection[ PPS_REMOVE_PPT_VIDEO_SUBSCRIPTIONS ] = (gcnew ::System::Data::SqlClient::SqlCommand());
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_REMOVE_PPT_VIDEO_SUBSCRIPTIONS ]))->CommandText = L"dbo.uspPublishingPointDestinationRemove";
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_REMOVE_PPT_VIDEO_SUBSCRIPTIONS ]))->CommandType = ::System::Data::CommandType::StoredProcedure;
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_REMOVE_PPT_VIDEO_SUBSCRIPTIONS ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@RETURN_VALUE", 
                ::System::Data::SqlDbType::Int, 4, ::System::Data::ParameterDirection::ReturnValue, 10, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
        (cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_REMOVE_PPT_VIDEO_SUBSCRIPTIONS ]))->Parameters->Add((gcnew ::System::Data::SqlClient::SqlParameter(L"@PptParticipantID", 
                ::System::Data::SqlDbType::UniqueIdentifier, 16, ::System::Data::ParameterDirection::Input, 0, 0, nullptr, ::System::Data::DataRowVersion::Current, 
                false, nullptr, L"", L"", L"")));
		return;
}

CPublishingPointDestinationMs::~CPublishingPointDestinationMs(void)
{
// add code to iterate through array of commands and close all and delete objects...
	return;
}

DWORD CPublishingPointDestinationMs::EstablishVideoSubscription(
		PIP_HEADER pIpHeader
		, PUDP_HEADER pUdpHeader
		, PEVENT_HEADER pEventHeader
		, PEVENT_PACKET_HEADER pEventPacketHeader
		,  PESTABLISH_VIDEO_SUBSCRIPTION pEstablishMediaDestination
		)
{
	::System::Data::SqlClient::SqlCommand^  command;
	::System::Int32 returnValue;

	try
	{

		command = 
			(cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_ESTABLISH_VIDEO_SUBSCRIPTION ]));

		System::Threading::Monitor::Enter( command );
	
		//ServiceConfig^ config = gcnew ServiceConfig();
		//config->Transaction = TransactionOption::RequiresNew;

		command->Connection = gcnew ::System::Data::SqlClient::SqlConnection( m_ConnStr );

		command->Parameters[ L"@ConferenceID" ]->Value = 
			Utilities::FromGUID( pEventPacketHeader->publishingPointId );
		command->Parameters[ L"@ConferenceParticipantID" ]->Value = 
			Utilities::FromGUID( pEventPacketHeader->pptParticipantId );
		command->Parameters[ L"@ipVerLen" ]->Value = 
			static_cast<System::Int16>( pIpHeader->ip_verlen );
		command->Parameters[ L"@ipTos" ]->Value = 
			static_cast<System::Int16>( pIpHeader->ip_tos );
		command->Parameters[ L"@ipTotalLength" ]->Value = 
			static_cast<System::Int32>( pIpHeader->ip_totallength );
		command->Parameters[ L"@ipId" ]->Value = 
			static_cast<System::Int32>( pIpHeader->ip_id );
		command->Parameters[ L"@ipOffset" ]->Value = 
			static_cast<System::Int32>( pIpHeader->ip_offset );
		command->Parameters[ L"@ipTtl" ]->Value = 
			static_cast<System::Int16>( pIpHeader->ip_ttl );
		command->Parameters[ L"@ipProtocol" ]->Value = 
			static_cast<System::Int16>( pIpHeader->ip_protocol );
		command->Parameters[ L"@srcIP" ]->Value = 
			static_cast<System::Int64>( pIpHeader->ip_srcaddr );
		command->Parameters[ L"@destIP" ]->Value = 
			static_cast<System::Int64>( pIpHeader->ip_destaddr );
		command->Parameters[ L"@srcPort" ]->Value = 
			static_cast<System::Int32>( pUdpHeader->src_portno );
		command->Parameters[ L"@destPort" ]->Value = 
			static_cast<System::Int32>( pUdpHeader->dst_portno );
		command->Parameters[ L"@udpLength" ]->Value = 
			static_cast<System::Int32>( pUdpHeader->udp_length );
		command->Parameters[ L"@MacAddress" ]->Value = 
			Marshal::PtrToStringAnsi( (IntPtr)pEventHeader->eventMacAddress );

		::System::Data::ConnectionState previousConnectionState = command->Connection->State;
		if ((command->Connection->State & ::System::Data::ConnectionState::Open) 
					!= ::System::Data::ConnectionState::Open) {
			command->Connection->Open();
		}

		try {
			//ServiceDomain::Enter( config );
			//command->Connection->EnlistDistributedTransaction( 
			//	cli::safe_cast<::System::EnterpriseServices::ITransaction^  >( ContextUtil::Transaction )
			//	);

			returnValue = command->ExecuteNonQuery();

			//ContextUtil::SetComplete();
		}
		catch( Exception^ ex )
		{
			//ContextUtil::SetAbort();
		}
		finally 
		{
			//ServiceDomain::Leave();
			////SqlConnection::ClearPool( command->Connection );
			command->Connection->Close();
		}
	}
	finally
	{
		System::Threading::Monitor::Exit( command );
	}
	return returnValue;
}

DWORD CPublishingPointDestinationMs::ReadVideoSubscription(
		GUID ID
		, PIP_HEADER pIpHeader
		, PUDP_HEADER pUdpHeader
		, PEVENT_HEADER pEventHeader
		, PEVENT_PACKET_HEADER pEventPacketHeader
		, PESTABLISH_VIDEO_SUBSCRIPTION pEstablishVideoSubscription
		)
{
	::System::Data::SqlClient::SqlCommand^  command;
	::System::Int32 returnValue;

	try
	{

		command = 
		(cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_READ_PPT_VIDEO_SUBSCRIPTIONS ]));
	
		System::Threading::Monitor::Enter( command );

		//ServiceConfig^ config = gcnew ServiceConfig();
		//config->Transaction = TransactionOption::Supported;

		command->Connection = gcnew ::System::Data::SqlClient::SqlConnection( m_ConnStr );
	
		command->Parameters[ L"@PublishingPointId" ]->Value = 
			Utilities::FromGUID( ID );

		::System::Data::ConnectionState previousConnectionState = command->Connection->State;
		if ((command->Connection->State & ::System::Data::ConnectionState::Open) 
					!= ::System::Data::ConnectionState::Open) {
			command->Connection->Open();
		}
		try 
		{
			//ServiceDomain::Enter( config );

			SqlDataReader^ reader = command->ExecuteReader();

			try
			{
				while( reader->Read() )
				{
					pIpHeader->ip_verlen			= static_cast<INT16>( reader[ L"ipVerLen" ] );
					pIpHeader->ip_tos				= static_cast<INT16>( reader[ L"ipTos" ] );
					pIpHeader->ip_totallength		= static_cast<INT32>( reader[ L"ipTotalLength" ] );
					pIpHeader->ip_id				= static_cast<INT32>( reader[ L"ipId" ] );
					pIpHeader->ip_offset			= static_cast<INT32>( reader[ L"ipOffset" ] );
					pIpHeader->ip_ttl				= static_cast<INT16>( reader[ L"ipTtl" ] );
					pIpHeader->ip_protocol			= static_cast<INT16>( reader[ L"ipProtocol" ] );
					pIpHeader->ip_srcaddr			= static_cast<INT64>( reader[ L"srcIP" ] );
					pIpHeader->ip_destaddr			= static_cast<INT64>( reader[ L"destIP" ] );

					pUdpHeader->src_portno			= static_cast<INT32>( reader[ L"srcPort" ] );
					pUdpHeader->dst_portno			= static_cast<INT32>( reader[ L"destPort" ] );
					pUdpHeader->udp_length			= static_cast<INT32>( reader[ L"udpLength" ] );
					pUdpHeader->udp_checksum		= 0;

					::CoCreateGuid( &pEventHeader->eventId ); 
					pEventHeader->eventType			= BE_ESTABLISH_VIDEO_SUBSCRIPTION;
					pEventHeader->eventTypeId		= BET_MANAGEMENT;

					PCHAR str2 = (PCHAR)(PVOID)Marshal::StringToHGlobalAnsi( reader->GetString( reader->GetOrdinal( L"MacAddress" ) ) );
					strncpy_s(
						pEventHeader->eventMacAddress 
						, _countof( pEventHeader->eventMacAddress ) //- 1
						, str2
						, _TRUNCATE
						);
					Marshal::FreeHGlobal( static_cast<System::IntPtr>( str2 ) );

					::CoCreateGuid( &pEventPacketHeader->securityToken );
					pEventPacketHeader->eventWsaBufType = PDBT_EVENT_PACKET_PAYLOAD;
					pEventPacketHeader->eventPublishingPoint = static_cast<INT32>( reader[ L"SessionID" ] );
					pEventPacketHeader->publishingPointId = 
						Utilities::ToGUID( reader->GetGuid( reader->GetOrdinal( L"PublishingPointId" ) ) );
					pEventPacketHeader->pptParticipantId = 
						Utilities::ToGUID( reader->GetGuid( reader->GetOrdinal( L"PptParticipantId" ) ) );
					pEventPacketHeader->dataBlobSize = MAX_ESTABLISH_VIDEO_SUBSCRIPTION;
				}
			}
			finally
			{
				delete reader;
			}
		}
		finally 
		{
			////SqlConnection::ClearPool( command->Connection );
			//ServiceDomain::Leave();
			command->Connection->Close();

		}
	}
	finally
	{
		System::Threading::Monitor::Exit( command );
	}
	DWORD dwReturnValue = returnValue;
    return dwReturnValue;
}

DWORD CPublishingPointDestinationMs::RemoveMediaDestination(
		PIP_HEADER pIpHeader
		, PUDP_HEADER pUdpHeader
		, PEVENT_HEADER pEventHeader
		, PEVENT_PACKET_HEADER pEventPacketHeader
		, PPACKET_DATA_BLOB pPacketDataBlob
		)
{
	::System::Data::SqlClient::SqlCommand^  command;
	::System::Int32 returnValue;

	try
	{

		command = 
		(cli::safe_cast<::System::Data::SqlClient::SqlCommand^  >(this->m_commandCollection[ PPS_REMOVE_PPT_VIDEO_SUBSCRIPTIONS ]));
	
		System::Threading::Monitor::Enter( command );

		//ServiceConfig^ config = gcnew ServiceConfig();
		//config->Transaction = TransactionOption::RequiresNew;
	
		command->Connection = gcnew ::System::Data::SqlClient::SqlConnection( m_ConnStr );

		command->Parameters[ L"@PptParticipantID" ]->Value = 
			Utilities::FromGUID( pEventPacketHeader->pptParticipantId );

		::System::Data::ConnectionState previousConnectionState = command->Connection->State;
		if ((command->Connection->State & ::System::Data::ConnectionState::Open) 
					!= ::System::Data::ConnectionState::Open) {
			command->Connection->Open();
		}

		try 
		{
			//ServiceDomain::Enter( config );
			//command->Connection->EnlistDistributedTransaction( 
			//	cli::safe_cast<::System::EnterpriseServices::ITransaction^  >( ContextUtil::Transaction )
			//	);

			returnValue = command->ExecuteNonQuery();

			//ContextUtil::SetComplete();
		}
		catch( Exception^ ex )
		{
			//ContextUtil::SetAbort();
		}
		finally 
		{
			//ServiceDomain::Leave();
			////SqlConnection::ClearPool( command->Connection );
			command->Connection->Close();
		}
	}
	finally
	{
		System::Threading::Monitor::Exit( command );
	}

	return returnValue;
}
