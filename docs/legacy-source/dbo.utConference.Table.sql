/****** Object:  Table [dbo].[utConference]    Script Date: 08/27/2008 14:35:22 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[utConference](
	[ID] [uniqueidentifier] NOT NULL CONSTRAINT [DF__utConference__ID__62EF9734]  DEFAULT (newid()),
	[Name] [nvarchar](50) NOT NULL CONSTRAINT [DF__utConferen__Name__63E3BB6D]  DEFAULT ('Name'),
	[Description] [nvarchar](50) NOT NULL CONSTRAINT [DF__utConfere__Descr__64D7DFA6]  DEFAULT ('ROLE'),
	[ConferenceType] [uniqueidentifier] NOT NULL CONSTRAINT [DF__utConfere__Confe__65CC03DF]  DEFAULT (newid()),
	[StartTime] [datetime] NOT NULL CONSTRAINT [DF__utConfere__Start__66C02818]  DEFAULT (getdate()),
	[EndTime] [datetime] NOT NULL CONSTRAINT [DF__utConfere__EndTi__67B44C51]  DEFAULT (getdate()),
	[timeStamp] [datetime] NOT NULL CONSTRAINT [DF__utConfere__timeS__68A8708A]  DEFAULT (getdate()),
	[isActive] [int] NOT NULL,
	[LongDescription] [nvarchar](1024) NOT NULL,
	[SessionID] [int] NOT NULL,
 CONSTRAINT [PK_utConference] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
ALTER TABLE [dbo].[utConference]  WITH CHECK ADD  CONSTRAINT [FK_utConference_utSession] FOREIGN KEY([SessionID])
REFERENCES [dbo].[utSession] ([InUseID])
GO
ALTER TABLE [dbo].[utConference] CHECK CONSTRAINT [FK_utConference_utSession]
GO
ALTER TABLE [dbo].[utConference]  WITH CHECK ADD  CONSTRAINT [FK_utConferenceType_utConference_U] FOREIGN KEY([ConferenceType])
REFERENCES [dbo].[utConferenceType] ([ID])
GO
ALTER TABLE [dbo].[utConference] CHECK CONSTRAINT [FK_utConferenceType_utConference_U]
GO
