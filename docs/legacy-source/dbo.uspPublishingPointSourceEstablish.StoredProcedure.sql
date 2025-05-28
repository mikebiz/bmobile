/****** Object:  StoredProcedure [dbo].[uspPublishingPointSourceEstablish]    Script Date: 08/27/2008 14:36:25 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Description:	Establish a video source for a Publishing Point
-- =============================================
CREATE PROCEDURE [dbo].[uspPublishingPointSourceEstablish]
(
	@ConferenceID uniqueidentifier 
	, @ConferenceParticipantID uniqueidentifier 
	, @srcIP bigint
	, @destIP bigint
	, @srcPort int
	, @destPort int
	, @majorType uniqueidentifier
	, @subType uniqueidentifier
	, @fixedSizeSamples int
	, @temporalCompression int
	, @sampleSize bigint
	, @headerFormatType uniqueidentifier
	, @headerFormatSize bigint
	, @rcSourceLeft bigint
	, @rcSourceTop bigint
	, @rcSourceRight bigint
	, @rcSourceBottom bigint
	, @rcTargetLeft bigint
	, @rcTargetTop bigint 
	, @rcTargetRight bigint 
	, @rcTargetBottom bigint 
	, @dwBitRate bigint
	, @dwBitErrorRate bigint
	, @AvgTimePerFrame bigint
	, @biSize bigint
	, @biHeight bigint 
	, @biWidth bigint 
	, @biPlanes int
	, @biBitCount int
	, @biCompression bigint 
	, @biSizeImage bigint
	, @biXPelsPerMeter bigint 
	, @biYPelsPerMeter bigint
	, @biClrUsed bigint
	, @biClrImportant bigint 
	, @wFormatTag int 
	, @nChannels int
	, @nSamplesPerSec bigint
	, @nAvgBytesPerSec bigint
	, @nBlockAlign int 
	, @wBitsPerSample int 
	, @cbSize int 
	, @MacAddress char( 24 )
)
AS
BEGIN
	SET NOCOUNT ON;

	DECLARE	@_error int
	DECLARE @_errorString char( 255 )

	DECLARE @_rowCount int

	DECLARE	@_MacAddress char( 24 )
	DECLARE @_pptsID uniqueidentifier
	DECLARE @_cdrID uniqueidentifier

	SET @_error = 0
	SET @_rowCount = 0

--NOTE:  Need to declare error codes and do this correctly...

	IF NOT EXISTS
	( 
		SELECT TOP 1 PPTP.ID
		FROM utConferenceParticipant AS PPTP
		WHERE PPTP.ID = @ConferenceParticipantID
	)
	BEGIN
		
		SET @_errorString = CONVERT( UNIQUEIDENTIFIER, CONVERT( CHAR( 255 ), @ConferenceParticipantID ) )
		RAISERROR( 
			'PublishingPoint Participant does not exist with id: %s'
			, 16
			, 1
			, @_errorString
			) WITH LOG
		RETURN -1

	END
	ELSE
	BEGIN

		SELECT @_MacAddress = PPTP.MacAddress
		FROM utConferenceParticipant AS PPTP
		WHERE PPTP.ID = @ConferenceParticipantID

		IF @_MacAddress <> @MacAddress
		BEGIN
			SET @_MacAddress = @MacAddress

			UPDATE utConferenceParticipant
			SET MacAddress = @MacAddress
		END

	END

	IF EXISTS
	(
		SELECT TOP 1 PPTS.ID
		FROM utPublishingPointSource AS PPTS
		WHERE PPTS.ConferenceParticipantID = @ConferenceParticipantID
	)
	BEGIN
	
		SELECT @_pptsID = PPTS.ID
		FROM utPublishingPointSource AS PPTS
		WHERE PPTS.ConferenceParticipantID = @ConferenceParticipantID

		EXECUTE @_error = uspDeletePublishingPointSource
			@_pptsID

		IF 0 <> @_error
		BEGIN
			SET @_errorString = CONVERT( UNIQUEIDENTIFIER, CONVERT( CHAR( 255 ), @ConferenceParticipantID ) )
			RAISERROR( 
				'Delete of PublishingPointSource failed with error code of: %d for PublishingPoint Participant: %s'
				, 16
				, 1
				, @_error
				, @_errorString
				) WITH LOG
			RETURN -1
		END
		
	END

	SET @_pptsID = newid()
	EXECUTE @_error = uspCreatePublishingPointSource
			@_pptsID
			, @ConferenceParticipantID  
			, @srcIP 
			, @destIP 
			, @srcPort 
			, @destPort 
			, @majorType 
			, @subType
			, @fixedSizeSamples 
			, @temporalCompression 
			, @sampleSize 
			, @headerFormatType
			, @headerFormatSize
			, @rcSourceLeft
			, @rcSourceTop 
			, @rcSourceRight 
			, @rcSourceBottom
			, @rcTargetLeft 
			, @rcTargetTop  
			, @rcTargetRight 
			, @rcTargetBottom 
			, @dwBitRate
			, @dwBitErrorRate 
			, @AvgTimePerFrame
			, @biSize
			, @biHeight
			, @biWidth
			, @biPlanes
			, @biBitCount
			, @biCompression
			, @biSizeImage
			, @biXPelsPerMeter 
			, @biYPelsPerMeter
			, @biClrUsed 
			, @biClrImportant
			, @wFormatTag
			, @nChannels
			, @nSamplesPerSec
			, @nAvgBytesPerSec
			, @nBlockAlign
			, @wBitsPerSample
			, @cbSize

	IF 0 <> @_error
	BEGIN

		SET @_errorString = CONVERT( UNIQUEIDENTIFIER, CONVERT( CHAR( 255 ), @ConferenceParticipantID ) )
		RAISERROR( 
			'Insert of PublishingPointSource failed with error code of: %d for PublishingPoint Participant: %s'
			, 16
			, 1
			, @_error
			, @_errorString
			) WITH LOG
		RETURN -1

	END

--	SET @_cdrID = newid()
--	complete CDR stuff once datamodel is finalized.

	RETURN 0
END
GO
